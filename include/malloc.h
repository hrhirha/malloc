#ifndef MALLOC_H
# define MALLOC_H
# include <sys/mman.h>
# include <unistd.h>
# include <pthread.h>

# define MMAP_PROT			(PROT_READ|PROT_WRITE)
# define MMAP_FLAGS			(MAP_PRIVATE|MAP_ANONYMOUS)
# define ALIGNTO			(0x2 * sizeof(size_t))
# define PAGE_SIZE			(getpagesize())
# define BLOCK_SIZE			(sizeof(struct s_block))
# define TINY_BLOCK_SIZE	(32)
# define SMALL_BLOCK_SIZE	(4096)
# define TINY_ZONE_SIZE		(ALIGN(TINY_BLOCK_SIZE * 100, PAGE_SIZE))
# define SMAL_ZONE_SIZE		(ALIGN(SMALL_BLOCK_SIZE * 100, PAGE_SIZE))

# define ALIGN(x, a)		((x + (a - 0x1)) & ~(a - 0x1))
# define INUSE(b)			(b->header & 0x1)
# define GET_SIZE(b)		(b->header & ~0x1)
# define BLOCK_HEAD(z)		((t_block)(z+1))
# define NEXT_BLOCK(b)		((t_block)((char*)b + GET_SIZE(b)))
# define PREV_BLOCK(b)		((t_block)((char *)b - b->prevsz))
# define IN_ZONE(b, z)		((char*)b > (char*)z && (char*)b < (char*)z + z->size)

typedef enum	e_type
{
	TINY,
	SMALL,
	LARGE
}				t_type;

typedef struct	s_zone
{
	size_t			size;
	size_t			free_size;
	size_t			blocks;
	t_type			type;
	struct s_zone	*next;
	struct s_zone	*prev;
}				*t_zone;

typedef struct	s_block
{
	size_t			prevsz;
	size_t			header;
}				*t_block;

/* global variable */

extern t_zone			g_zone;
extern pthread_mutex_t	g_lock;

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

void	show_alloc_mem(void);

/* zone.c */
t_zone	new_zone(size_t size, t_type type);
t_zone	add_zone(t_zone new);
t_zone	find_zone_by_ptr(void *ptr);
t_zone	get_zone_by_type(size_t size, t_type type);
void	unmap_zone(t_zone zone);

/* block.c */
t_block	new_block(size_t size, t_zone zone);
t_block	find_free_block(size_t size, t_type type);
t_block	resize_block(t_block block, t_zone zone, size_t size);
void	free_block(t_block block, t_zone zone);

/* utils.c */
void	ft_putn(size_t n, int base);
void	ft_puts(char *s);
void	*ft_memcpy(void *dest, const void *src, size_t n);

#endif
