#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
char	*ft_read_to_bookmark(int fd, char *bookmark);
size_t  ft_gnl_strlen(char *s);
char    *ft_gnl_strchr(char *s, int c);
char    *ft_gnl_strjoin(char *bookmark, char *buffer);
char    *ft_extract_line(char *bookmark);
char    *ft_update_bookmark(char *bookmark);

#endif
