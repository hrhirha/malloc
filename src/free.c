#include "malloc.h"

void	free(void *ptr)
{
	t_zone	zone;
	t_block	block;
	
	pthread_mutex_lock(&g_lock);
	if (ptr == NULL || (zone = find_zone_by_ptr(ptr)) == NULL)
	{
		pthread_mutex_unlock(&g_lock);
		return ;
	}
	block = (t_block)(ptr - BLOCK_SIZE);
	if (INUSE(block))
		free_block(block, zone);
	pthread_mutex_unlock(&g_lock);
	return ;
}
