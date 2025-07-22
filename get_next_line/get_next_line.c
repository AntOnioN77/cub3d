#include "get_next_line.h"

char	*ft_read_to_bookmark(int fd, char *bookmark)
{
	char	*buffer;
	int		read_bytes;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	if (!bookmark)
	{
		bookmark = (char *)malloc(sizeof(char));
		if (!bookmark)
		{
			free(buffer);
			return (NULL);
		}
		*bookmark = '\0';
	}
	read_bytes = 1;
	while (!ft_gnl_strchr(bookmark, '\n') && read_bytes != 0)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(buffer);
			free(bookmark);
			return (NULL);
		}
		if (read_bytes == 0)
			break ;
		buffer[read_bytes] = '\0';
		bookmark = ft_gnl_strjoin(bookmark, buffer);
		if (!bookmark)
		{
			free(buffer);
			return (NULL);
		}
	}
	free(buffer);
	return (bookmark);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*bookmark;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	bookmark = ft_read_to_bookmark(fd, bookmark);
	if (!bookmark)
		return (NULL);
	line = ft_extract_line(bookmark);
	bookmark = ft_update_bookmark(bookmark);
	if (!line && bookmark)
	{
		free(bookmark);
		bookmark = NULL;
		return (NULL);
	}
	return (line);
}
