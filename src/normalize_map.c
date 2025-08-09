#include "../inc/cub.h"

static int	get_max_len(char **map)
{
	int	i;
	int	max_len;

	i = 0;
	max_len = 0;
	while (map[i])
	{
		if ((int)ft_strlen(map[i]) > max_len)
			max_len = (int)ft_strlen(map[i]);
		i++;
	}
	return (max_len);
}

static void	copy_row(char **dst, char *src, int len)
{
	*dst = ft_calloc(len + 1, sizeof(char));
	if (!*dst)
		error_exit("Memory error trying to normalize map");
	ft_memset(*dst, ' ', len);
	ft_memcpy(*dst, src, ft_strlen(src));
}

char	**normalize_map(char **map)
{
	int		i;
	int		max_len;
	char	**new_map;

	max_len = get_max_len(map);
	i = 0;
	while (map[i])
		i++;
	new_map = malloc(sizeof(char *) * (i + 1));
	if (!new_map)
		error_exit("Memory error trying to normalize map");
	i = 0;
	while (map[i])
	{
		copy_row(&new_map[i], map[i], max_len);
		i++;
	}
	new_map[i] = NULL;
	return (new_map);
}
