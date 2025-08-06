#ifndef MALLOC_H
# define MALLOC_H
# include <sys/mman.h>
# include <unistd.h>

# define MMAP_PROT		(PROT_READ|PROT_WRITE)
# define MMAP_FLAG		(MAP_PRIVATE|MAP_ANONYMOUS)

# define ALIGNTO		(0x2 * sizeof(size_t))
# define PAGE_SIZE		getpagesize()
# define BLOCK_SIZE		0x10

# define ALIGN(x)		(x + (ALIGNTO - 0x1) & ~(ALIGNTO - 0x1))
# define INUSE(x)		(x->header & 0x1)
# define GET_SIZE(x)	(x->header & ~0x1)
# define BLOCK_HEAD(x)	((t_block)(x+1))
# define NEXT_BLOCK(x)	((t_block)((char*)x + GET_SIZE(x)))
# define PREV_BLOCK(x)	((t_block)((char *)x - x->prevsz))
# define IN_ZONE(b, z)	((size_t)b < (size_t)((char *)z + z->size))

# define TINY_BLKSZ		256
# define SMAL_BLKSZ		4096
# define TINY_ZONSZ		(TINY_BLKSZ / 32) * PAGE_SIZE
# define SMAL_ZONSZ		(SMAL_BLKSZ / 32) * PAGE_SIZE

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
	struct s_block	*next;
	struct s_block	*prev;
}				*t_block;

/* global variable */

extern t_zone	g_zone;

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

void	show_alloc_mem(void);

/* zone.c */
t_zone	new_zone(size_t size, t_type type);
t_zone	add_zone(t_zone new);
t_zone	find_zone_by_ptr(void *ptr);
t_zone	get_zone_by_type(size_t size, t_type type);
int		unmap_zone(t_zone zone);

/* block.c */
t_block	new_block(size_t size, t_zone zone);
t_block	find_free_block(size_t size, t_type type);
t_block	resize_block(t_block block, t_zone zone, size_t size);
void	free_block(void *ptr, t_zone zone);

/* utils.c */
void	putn(size_t n, int base);
void	ft_puts(char *s);
void	*ft_memcpy(void *dest, const void *src, size_t n);

#endif
