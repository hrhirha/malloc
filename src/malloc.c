#include "malloc.h"

t_zone	g_zone = NULL;

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
		putn((size_t)zone, 16);
		ft_puts("\n");
		while (GET_SIZE(block))
		{
			if (!INUSE(block))
			{
				block = NEXT_BLOCK(block);
				continue ;
			}
			total += GET_SIZE(block) - BLOCK_SIZE;
			putn((size_t)block, 16);
			ft_puts(" - ");
			putn((size_t)((char *)block + GET_SIZE(block)) - BLOCK_SIZE, 16);
			ft_puts(" : ");
			putn(GET_SIZE(block) - BLOCK_SIZE, 10);
			ft_puts(" bytes\n");
			block = NEXT_BLOCK(block);
		}
		zone = zone->next;
	}
	ft_puts("Total : ");
	putn(total, 10);
	ft_puts(" bytes\n");
}

void	*malloc(size_t size)
{
	t_zone	zone;
	t_block	block;
	t_type	type;

	size = ALIGN(size + BLOCK_SIZE);
	type = (size > TINY_BLKSZ) + (size > SMAL_BLKSZ);
	block = find_free_block(size, type);
	if (block == NULL && (zone = get_zone_by_type(size, type)))
	{
		block = new_block(size, zone);
	}
	if (block == NULL)
		return (NULL);
	return ((void *)((char *)block + BLOCK_SIZE));
}
