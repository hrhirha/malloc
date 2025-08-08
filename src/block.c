#include "malloc.h"

t_block	new_block(size_t size, t_zone zone)
{
	t_block	block;

	block = BLOCK_HEAD(zone);
	while (IN_ZONE(NEXT_BLOCK(block), zone) && GET_SIZE(block))
	{
		block = NEXT_BLOCK(block);
	}
	block->prevsz = 0;
	block->header = size | 0x1;
	zone->free_size -= size;
	zone->blocks += 1;
	return (block);
}

void	split_block(t_block block, size_t size)
{
	t_block	next;
	size_t	block_size;

	if (GET_SIZE(block) >= size + BLOCK_SIZE + 0x10)
	{
		block_size = GET_SIZE(block);
		block->header = size;
		next = NEXT_BLOCK(block);
		next->prevsz = 0;
		next->header = block_size - size;
	}
}

t_block	find_free_block(size_t size, t_type type)
{
	t_zone	zone;
	t_block	block;

	zone = g_zone;
	while (zone)
	{
		if (zone->type == type && zone->free_size > size)
		{
			block = BLOCK_HEAD(zone);
			while (IN_ZONE(block, zone) && GET_SIZE(block))
			{
				if (!INUSE(block) && GET_SIZE(block) >= size)
				{
					split_block(block, size);
					block->header |= 0x1;
					zone->blocks++;
					return (block);
				}
				block = NEXT_BLOCK(block);
			}
		}
		zone = zone->next;
	}
	return (NULL);
}

t_block	merge_block(t_block block, t_zone zone)
{
	t_block	prev;
	t_block	next;

	if (block->prevsz != 0)
	{
		prev = PREV_BLOCK(block);
		prev->header += block->header;
		block->header = 0;
		block->prevsz = 0;
		block = prev;
	}
	next = NEXT_BLOCK(block);
	if (IN_ZONE(next, zone) && !INUSE(next))
	{
		block->header += next->header;
		if (GET_SIZE(next) == 0)
			block->header += (size_t)((char *)zone + zone->size) - (size_t)next;
		next->header = 0;
		next->prevsz = 0;
	}
	return (block);
}

t_block	resize_block(t_block block, t_zone zone, size_t size)
{
	t_block next;

	block->header &= ~0x1;
	if (size > GET_SIZE(block))
	{
		next = NEXT_BLOCK(block);
		if (!IN_ZONE(next, zone) || INUSE(next))
		{
			block->header |= 0x1;
			return (NULL);
		}
		if (GET_SIZE(next) == 0)
			block->header += (size_t)(((char *)zone + zone->size - (size_t)next));
		else
			block->header += next->header;
	}
	split_block(block, size);
	block->header |= 0x1;
	return (block);
}

void	free_block(t_block block, t_zone zone)
{
	t_block	next;

	if (zone == NULL)
		return ;
	block->header &= ~0x1;
	block = merge_block(block, zone);
	next = NEXT_BLOCK(block);
	if (IN_ZONE(next, zone) && INUSE(next))
	{
		next->prevsz = block->header;
	}
	zone->blocks--;
	zone->free_size += GET_SIZE(block);
	if (zone->blocks == 0)
		unmap_zone(zone);
}
