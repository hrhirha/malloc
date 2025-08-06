#include "malloc.h"

void	free(void *ptr)
{
	t_zone	zone;

	if (ptr == NULL)
		return ;
	zone = find_zone_by_ptr(ptr);
	free_block(ptr, zone);
}
