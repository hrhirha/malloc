ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC = gcc
CFLAGS = -Iinclude -fPIC -lpthread -Wall -Wextra -Werror -O0 -g #-fsanitize=address
NAME = libft_malloc_$(HOSTTYPE).so
LIBNAME = libft_malloc.so
SRC =	malloc.c  \
		free.c    \
		realloc.c \
		zone.c    \
		block.c   \
		utils.c
OBJ = $(patsubst %,src/%,$(SRC:.c=.o))
ARGS = $(wordlist 2, $(words $(MAKECMDGOALS)), $(MAKECMDGOALS))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -shared $^ -o $@
	ln -s $(NAME) $(LIBNAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

test: $(NAME)
	@gcc -O0 -Iinclude main.c -o mal
	@LD_PRELOAD=./$(NAME) LD_LIBRARY_PATH=. ASAN_OPTIONS=verify_asan_link_order=0 ./mal
	@#rm ./mal

cmd: $(NAME)
	@LD_PRELOAD=./$(NAME) ASAN_OPTIONS=verify_asan_link_order=0 $(ARGS)

re: fclean all
