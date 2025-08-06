ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC = gcc
CFLAGS = -Iinclude -fPIC -g #-fsanitize=address # -Wall -Wextra -Werror
NAME = libft_malloc_$(HOSTTYPE).so
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

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

test: $(NAME)
	@gcc -O0 -g -Iinclude main.c -o mal
	@LD_PRELOAD=./$(NAME) ./mal
	@#rm ./mal

cmd: $(NAME)
	@LD_PRELOAD=./$(NAME) ASAN_OPTIONS=verify_asan_link_order=0 $(ARGS)

re: fclean all
