#include "malloc.h"

t_zone	new_zone(size_t size, t_type type)
{
	size_t	zone_size;
	t_zone	new;

	zone_size = size + (PAGE_SIZE - 0x1) & ~(PAGE_SIZE - 0x1);
	if (type == TINY)
		zone_size = TINY_ZONSZ;
	if (type == SMALL)
		zone_size = SMAL_ZONSZ;
	new = (t_zone)mmap(0, zone_size, MMAP_PROT, MMAP_FLAG, -1, 0);
	if (new == MAP_FAILED)
		return (NULL);
	new->size = zone_size;
	new->free_size = zone_size;
	new->blocks = 0;
	new->type = type;
	return (new);
}

t_zone	add_zone(t_zone new)
{
	t_zone	tmp;

	if (new == NULL)
		return (NULL);
	tmp = g_zone;
	while (tmp)
		tmp = tmp->next;
	new->next = NULL;
	new->prev = tmp;
	if (tmp)
		tmp->next = new;
	else
		g_zone = new;
	return (new);
}

t_zone	find_zone_by_ptr(void *ptr)
{
	t_zone	tmp;

	tmp = g_zone;
	while (tmp)
	{
		if ((size_t)ptr > (size_t)tmp && (size_t)ptr < (size_t)tmp + tmp->size)
			break ;
		tmp = tmp->next;
	}
	return (tmp);
}

t_zone	get_zone_by_type(size_t size, t_type type)
{
	t_zone	zone;

	zone = g_zone;
	while (zone && (zone->type != type || zone->free_size < size))
		zone = zone->next;
	if (zone == NULL)
		zone = add_zone(new_zone(size, type));
	if (zone == NULL)	
		return (NULL);
	return zone;
}

int	unmap_zone(t_zone zone)
{
	if (zone->next)
		zone->next->prev = zone->prev;
	if (zone->prev)
		zone->prev->next = zone->next;
	if (zone == g_zone)
		g_zone = zone->next;
	zone->next = NULL;
	zone->prev = NULL;
	munmap(zone, zone->size);
}
