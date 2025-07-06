NAME = libft.a
CC = cc
SOURCES = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c\
ft_isascii.c ft_isdigit.c ft_isprint.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c\
ft_memset.c ft_strchr.c ft_strrchr.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strncmp.c\
ft_strnstr.c ft_strdup.c ft_tolower.c ft_toupper.c ft_substr.c ft_strjoin.c ft_split.c\
ft_itoa.c ft_putnbr_fd.c ft_putendl_fd.c ft_putchar_fd.c ft_putstr_fd.c ft_strmapi.c\
ft_striteri.c ft_strtrim.c ft_putnbr_base.c ft_putunbr_base.c ft_strcmp.c ft_atod.c\
ft_isvalid_double.c
OBJECTS =$(subst .c,.o,$(SOURCES))
FLAGS = -Wall -Wextra -Werror
$(NAME): $(OBJECTS)
	ar rc $(NAME) $(OBJECTS)
	ranlib $(NAME)
ft_atoi.o: ft_atoi.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_atod.o: ft_atod.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_bzero.o: ft_bzero.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_calloc.o: ft_calloc.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_isalnum.o: ft_isalnum.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_isalpha.o: ft_isalpha.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_isascii.o: ft_isascii.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_isdigit.o: ft_isdigit.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_isprint.o: ft_isprint.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_memchr.o: ft_memchr.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_memcmp.o: ft_memcmp.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_memcpy.o: ft_memcpy.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_memmove.o: ft_memmove.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_memset.o: ft_memset.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_strchr.o: ft_strchr.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_strrchr.o: ft_strrchr.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_strlcat.o: ft_strlcat.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_strlcpy.o: ft_strlcpy.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_strlen.o: ft_strlen.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_strncmp.o: ft_strncmp.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_strcmp.o: ft_strcmp.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_strnstr.o: ft_strnstr.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_strdup.o: ft_strdup.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_tolower.o: ft_tolower.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_toupper.o: ft_toupper.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_substr.o: ft_substr.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_strjoin.o: ft_strjoin.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_split.o: ft_split.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_itoa.o: ft_itoa.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_putnbr_fd.o: ft_putnbr_fd.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_putendl_fd.o: ft_putendl_fd.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_putchar_fd.o: ft_putchar_fd.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_putstr_fd.o: ft_putstr_fd.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_strmapi.o: ft_strmapi.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_striteri.o: ft_striteri.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_strtrim.o: ft_strtrim.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_putnbr_base.o: ft_putnbr_base.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
ft_putunbr_base.o: ft_putunbr_base.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
.PHONY: clean fclean all re
all: $(NAME)
clean:
	rm -f $(OBJECTS)
fclean: clean
	rm -f $(NAME)
re: fclean all
