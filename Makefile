NAME = cub3D
CC = cc
SOURCES = main.c utils_for_mlx.c render.c check_loock.c MOOKS.c 
OBJECTS = $(SOURCES:.c=.o)
HEADERS = fractol.h
MLX_DIR = ./minilibx-linux/
MLX_LIB = $(addprefix $(MLX_DIR),libmlx_Linux.a)
LIBFT_DIR = ./libft/
LIBFT_LIB = $(addprefix $(LIBFT_DIR),libft.a)
CFLAGS = -Wall -Wextra -Werror -flto
CPPFLAGS = -Ilibft -Iminilibx-linux
LDFLAGS = -Llibft -Lminilibx-linux
LDLIBS = -lmlx_Linux -lXext -lX11 -lm -lz -lft
OP_FLAGS = -O3 -ffast-math -march=native
FSAN_FLAGS = -g3 -fsanitize=address
all: $(MLX_LIB) $(LIBFT_LIB) $(NAME)
$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
$(MLX_LIB):
	make -C $(MLX_DIR) 
$(LIBFT_LIB):
	make -C $(LIBFT_DIR) > /dev/null 2>&1
optimized: CFLAGS += $(OP_FLAGS)
optimized: fclean all
leaks: CFLAGS += $(FSAN_FLAGS)
leaks: fclean all
debug: CFLAGS += -g3
debug: clean all
clean:
	$(MAKE) -C $(MLX_DIR) clean
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJECTS)
fclean: clean
	$(MAKE) -C $(MLX_DIR) clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(MLX_LIB)
	rm -f $(NAME)
re: fclean all
.PHONY: all clean fclean re