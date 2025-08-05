#include "../inc/cub.h"

void	error_exit(const char *msg)
{
	ft_putstr_fd(ERR_PREFIX, STDERR_FILENO);
	ft_putstr_fd((char *)msg, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	exit(EXIT_FAILURE);
}

size_t	ft_strarr_len(char **arr)
{
	size_t	n;

	n = 0;
	if (!arr)
		return (0);
	while (arr[n])
		n++;
	return (n);
}

void	ft_strarr_free(char **arr)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	**ft_strarr_append(char **arr, const char *new_str)
{
	size_t	count;
	char	**new_arr;
	size_t	i;

	count = ft_strarr_len(arr);
	new_arr = malloc(sizeof(char *) * (count + 2));
	if (!new_arr)
		error_exit("Error de memoria en ft_strarr_append");
	i = 0;
	while (i < count)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[count] = ft_strdup(new_str);
	new_arr[count + 1] = NULL;
	free(arr);
	return (new_arr);
}

char	**normalize_map(char **map)
{
	int		i;
	int		max_len;
	char	**new_map;

	i = 0;
	max_len = 0;
	while (map[i])
	{
		if ((int)ft_strlen(map[i]) > max_len)
			max_len = (int)ft_strlen(map[i]);
		i++;
	}
	new_map = malloc(sizeof(char *) * (i + 1));
	if (!new_map)
		error_exit("Error de memoria al normalizar el mapa");
	i = 0;
	while (map[i])
	{
		new_map[i] = ft_calloc(max_len + 1, sizeof(char));
		if (!new_map[i])
			error_exit("Error de memoria al normalizar");
		ft_memset(new_map[i], ' ', max_len);
		ft_memcpy(new_map[i], map[i], ft_strlen(map[i]));
		i++;
	}
	new_map[i] = NULL;
	return (new_map);
}