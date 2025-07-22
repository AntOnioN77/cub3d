#include "../inc/cub.h"

void	error_exit(const char *msg)
{
	ft_putstr_fd(ERR_PREFIX, STDERR_FILENO);
	ft_putstr_fd((char *)msg, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	exit(EXIT_FAILURE);
}

static size_t	ft_strarr_len(char **arr)
{
	size_t	n;

	n = 0;
	if (!arr)
		return (0);
	while (arr[n])
		n++;
	return (n);
}

static void	ft_strarr_free(char **arr)
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

static int	parse_rgb(const char *s)
{
	char	**parts;
	int		r;
	int		g;
	int		b;

	parts = ft_split((char *)s, ',');
	if (!parts || ft_strarr_len(parts) != 3)
		error_exit("Color inválido, formato R,G,B");
	r = ft_atoi(parts[0]);
	g = ft_atoi(parts[1]);
	b = ft_atoi(parts[2]);
	ft_strarr_free(parts);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		error_exit("Componente de color fuera de rango 0-255");
	return ((r << 16) | (g << 8) | b);
}

static void	parse_line_config(char *line, t_config *cfg, int *count)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		cfg->tex_no = ft_strdup(line + 3);
	else if (ft_strncmp(line, "SO ", 3) == 0)
		cfg->tex_so = ft_strdup(line + 3);
	else if (ft_strncmp(line, "WE ", 3) == 0)
		cfg->tex_we = ft_strdup(line + 3);
	else if (ft_strncmp(line, "EA ", 3) == 0)
		cfg->tex_ea = ft_strdup(line + 3);
	else if (ft_strncmp(line, "F ", 2) == 0)
		cfg->floor_color = parse_rgb(line + 2);
	else if (ft_strncmp(line, "C ", 2) == 0)
		cfg->ceil_color = parse_rgb(line + 2);
	else
		error_exit("Entrada de configuración desconocida");
	(*count)++;
}

static char	*read_next_nonempty(int fd)
{
	char	*line;
	char	*trim;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			return (NULL);
		trim = ft_strtrim(line, "\n");
		free(line);
		if (*trim)
			return (trim);
		free(trim);
	}
}

static void	parse_config(int fd, t_config *cfg)
{
	int		count;
	char	*line;

	count = 0;
	while (count < 6)
	{
		line = read_next_nonempty(fd);
		if (!line)
			error_exit("Faltan líneas de configuración");
		parse_line_config(line, cfg, &count);
		free(line);
	}
}

char	**collect_map(int fd)
{
	char	**rows;
	char	*line;
	char	*trim;

	rows = NULL;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		trim = ft_strtrim(line, "\n");
		free(line);
		if (*trim)
		{
			rows = ft_strarr_append(rows, trim);
			free(trim);
		}
		else
			free(trim);
	}
	if (!rows)
		error_exit("Mapa vacío o faltante");
	return (rows);
}
void	check_extension(const char *file)
{
	size_t	len;

	len = ft_strlen(file);
	if (len < 4 || ft_strncmp(file + len - 4, ".cub", 4) != 0)
		error_exit("Error\nExtensión de fichero inválida (debe ser .cub)");
}

void	parse_file(const char *path, t_config *cfg)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		error_exit("No se pudo abrir archivo de configuración");
	parse_config(fd, cfg);
	cfg->map = collect_map(fd);
	close(fd);
}
