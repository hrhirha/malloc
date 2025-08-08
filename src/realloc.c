#include "malloc.h"

void	*realloc(void *ptr, size_t size)
{
	t_zone	zone;
	t_block	block;
	char	*p;

	if (ptr == NULL)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	pthread_mutex_lock(&g_lock);
	if ((zone = find_zone_by_ptr(ptr)))
	{
		size = ALIGN(size + BLOCK_SIZE, ALIGNTO);
		block = resize_block((t_block)(ptr - BLOCK_SIZE), zone, size);
		if (block != NULL)
		{
			pthread_mutex_unlock(&g_lock);
			return ((void *)((char *)block + BLOCK_SIZE));
		}
	}
	pthread_mutex_unlock(&g_lock);
	p = malloc(size);
	ft_memcpy(p, ptr, size);
	free(ptr);
	return (p);
}
