#include "malloc.h"

t_zone			g_zone = NULL;
pthread_mutex_t	g_lock;

void	show_alloc_mem(void)
{
	t_zone	zone;
	t_block	block;
	size_t	total;

	zone = g_zone;
	total = 0;
	while (zone)
	{
		block = BLOCK_HEAD(zone);
		if (zone->type == TINY) ft_puts("TINY: ");
		if (zone->type == SMALL) ft_puts("SMALL: ");
		if (zone->type == LARGE) ft_puts("LARGE: ");
		ft_putn((size_t)zone, 16);
		ft_puts("\n");
		while (GET_SIZE(block))
		{
			if (INUSE(block))
			{
				total += GET_SIZE(block) - BLOCK_SIZE;
				ft_putn((size_t)block, 16);
				ft_puts(" - ");
				ft_putn((size_t)((char *)block + GET_SIZE(block)) - BLOCK_SIZE, 16);
				ft_puts(" : ");
				ft_putn(GET_SIZE(block) - BLOCK_SIZE, 10);
				ft_puts(" bytes\n");
			}
			block = NEXT_BLOCK(block);
		}
		zone = zone->next;
	}
	ft_puts("Total : ");
	ft_putn(total, 10);
	ft_puts(" bytes\n");
}

void	*malloc(size_t size)
{
	t_zone	zone;
	t_block	block;
	t_type	type;

	pthread_mutex_init(&g_lock, NULL);
	pthread_mutex_lock(&g_lock);
	size = ALIGN(size + BLOCK_SIZE, ALIGNTO);
	type = (size > TINY_BLOCK_SIZE) + (size > SMALL_BLOCK_SIZE);
	block = find_free_block(size, type);
	if (block == NULL && (zone = get_zone_by_type(size, type)))
	{
		block = new_block(size, zone);
	}
	if (block == NULL)
	{
		pthread_mutex_unlock(&g_lock);
		return (NULL);
	}
	pthread_mutex_unlock(&g_lock);
	return ((void *)((char *)block + BLOCK_SIZE));
}
