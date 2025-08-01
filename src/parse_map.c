#include "../inc/cub.h"

static char	*adjust_map_line(char *line)
{
	char	*trim;
	char	*tmp;
	size_t	len;

	trim = ft_strtrim(line, "\n");
	if (!*trim)
		return (trim);
	len = ft_strlen(trim);
	if (trim[len - 1] != '1' && trim[len - 1] != ' ')
	{
		tmp = ft_strjoin(trim, " ");
		free(trim);
		return (tmp);
	}
	return (trim);
}

static char	**collect_map(int fd)
{
	char	**rows;
	char	*line;
	char	*adjusted;

	rows = NULL;
	line = get_next_line(fd);
	while (line)
	{
		adjusted = adjust_map_line(line);
		free(line);
		if (*adjusted)
			rows = ft_strarr_append(rows, adjusted);
		free(adjusted);
		line = get_next_line(fd);
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

void	parse_map(const char *path, t_config *cfg)
{
	int		fd;
	char	**norm;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		error_exit("No se pudo abrir archivo de configuración");
	parse_config(fd, cfg);
	cfg->map = collect_map(fd);
	norm = normalize_map(cfg->map);
	ft_strarr_free(cfg->map);
	cfg->map = norm;
	if (!has_valid_chars(cfg->map))
		error_exit("Mapa contiene caracteres inválidos");
	if (!is_map_closed(cfg->map))
		error_exit("El mapa no está cerrado");
	if (!has_one_player(cfg->map))
		error_exit("Debe haber exactamente un jugador");
	close(fd);
}