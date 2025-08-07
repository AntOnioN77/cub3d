RED     = \033[1;31m
GREEN   = \033[1;32m
BLUE    = \033[1;34m
RESET   = \033[0;0m
MAGENTA = \033[35m
CYAN    = \033[36m
BOLD    = \033[1m
PINK    = \033[38;5;206m

NAME        = cub3D
CC          = cc

SRC_DIR     = src
BIN_DIR     = bin
GNL_DIR     = get_next_line
SOURCES     = $(SRC_DIR)/main.c $(SRC_DIR)/utils_for_mlx.c $(SRC_DIR)/motor.c \
				$(SRC_DIR)/char_movement.c $(SRC_DIR)/map_check.c $(SRC_DIR)/column_printing.c\
				$(SRC_DIR)/parse_map.c $(SRC_DIR)/parse_map_cfg.c $(SRC_DIR)/map_check_utils.c\
				$(SRC_DIR)/memory_handler.c $(SRC_DIR)/motor_utils.c \
              $(GNL_DIR)/get_next_line.c \
              $(GNL_DIR)/get_next_line_utils.c
OBJECTS     = $(SOURCES:%.c=$(BIN_DIR)/%.o)
HEADERS     = inc/cub.h get_next_line/get_next_line.h

MLX_DIR     = ./minilibx-linux
MLX_LIB     = $(MLX_DIR)/libmlx_Linux.a
LIBFT_DIR   = ./libft
LIBFT_LIB   = $(LIBFT_DIR)/libft.a

CFLAGS      = -Wall -Wextra -Werror -flto
CPPFLAGS    = -Ilibft -Iminilibx-linux -I$(GNL_DIR)
LDFLAGS     = -Llibft -Lminilibx-linux
LDLIBS      = -lmlx_Linux -lXext -lX11 -lm -lz -lft

OP_FLAGS    = -O3 -ffast-math -march=native
FSAN_FLAGS  = -g3 -fsanitize=address

all: $(MLX_LIB) $(LIBFT_LIB) $(NAME)

$(NAME): $(OBJECTS)
	@printf "${BLUE}Linking binaries...${RESET}\n"
	@$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@
	@printf "${BOLD}${GREEN}${NAME} ${PINK}compiled successfully!${RESET}\n\n"

$(BIN_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(@D)
	@printf "${MAGENTA}Compiling ${CYAN}$<${RESET}\n"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(MLX_LIB):
	@make -s -C $(MLX_DIR) --no-print-directory > /dev/null 2>&1

$(LIBFT_LIB):
	@make -s -C $(LIBFT_DIR) --no-print-directory > /dev/null 2>&1

optimized: CFLAGS += $(OP_FLAGS)
optimized: fclean all

leaks: CFLAGS += $(FSAN_FLAGS)
leaks: fclean all

debug: CFLAGS += -g3
debug: clean all

clean:
	@rm -rf $(BIN_DIR)
	@make -s -C $(MLX_DIR) clean --no-print-directory > /dev/null 2>&1
	@make -s -C $(LIBFT_DIR) clean --no-print-directory > /dev/null 2>&1
	@printf "${RED}Cleaned object files!${RESET}\n"

fclean: clean
	@rm -f $(NAME)
	@printf "${BOLD}${RED}${NAME} deleted!${RESET}\n"

re: fclean all

.PHONY: all clean fclean re optimized leaks debug
