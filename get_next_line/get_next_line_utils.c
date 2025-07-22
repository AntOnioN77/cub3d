#include "get_next_line.h"

size_t  ft_gnl_strlen(char *s)
{
        size_t  i;

        i = 0;
        if (!s)
                return (0);
        while (s[i] != '\0')
                i++;
        return (i);
}

char    *ft_gnl_strchr(char *s, int c)
{
        int     i;

        i = 0;
        if (!s)
                return (0);
        if (c == '\0')
                return ((char *)&s[ft_gnl_strlen(s)]);
        while (s[i] != '\0')
        {
                if (s[i] == (char) c)
                        return ((char *)&s[i]);
                i++;
        }
        return (0);
}

char	*ft_gnl_strjoin(char *bookmark, char *buffer)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = 0;
	j = 0;
	if (!bookmark || !buffer)
		return (NULL);
	str = malloc(sizeof(char) * ((ft_gnl_strlen(bookmark) + ft_gnl_strlen(buffer)) + 1));
	if (!str)
	{
		free(bookmark);
		return (NULL);
	}
	while (bookmark[i])
	{
		str[i] = bookmark[i];
		i++;
	}
	while (buffer[j])
	{
		str[i + j] = buffer[j];
		j++;
	}
	str[i + j] = '\0';
	free(bookmark);
	return (str);
}

char    *ft_extract_line(char *bookmark)
{
        int             i;
        char    *str;

	i = 0;
        if (!bookmark || !bookmark[i])
                return (NULL);
        while (bookmark[i] && bookmark[i] != '\n')
                i++;
        str = (char *)malloc(sizeof(char) * (i + 2));
        if (!str)
                return (NULL);
	i = 0;
        while (bookmark[i] && bookmark[i] != '\n')
        {
                str[i] = bookmark[i];
		i++;
        }
        if (bookmark[i] == '\n')
                str[i++] = '\n';
        str[i] = '\0';
        return (str);
}

char    *ft_update_bookmark(char *bookmark)
{
	int		i;
	int		j;
	char    *str;

	if (!bookmark)
		return (NULL);
	i = 0;
        while (bookmark[i] && bookmark[i] != '\n')
                i++;
        if (!bookmark[i])
        {
                free(bookmark);
                return (NULL);
        }
        str = (char *)malloc(sizeof(char) * (ft_gnl_strlen(bookmark) - i + 1));
        if (!str)
	{
		free(bookmark);
		return (NULL);
	}
        i++;
	j = 0;
        while (bookmark[i])
                str[j++] = bookmark[i++];
        str[j] = '\0';
        free(bookmark);
        return (str);
}
