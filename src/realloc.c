#include "malloc.h"

void	*realloc(void *ptr, size_t size)
{
	t_zone	zone;
	t_block	block;
	char	*p;

	if (ptr == NULL || (zone = find_zone_by_ptr(ptr)) == NULL)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	block = (t_block)(ptr - BLOCK_SIZE);
	if ((block = resize_block(block, zone, size)) != NULL)
	{
		return ((void *)((char *)block + BLOCK_SIZE));
	}
	p = malloc(size);
	ft_memcpy(p, ptr, size);
	free(ptr);
	return (p);
}
