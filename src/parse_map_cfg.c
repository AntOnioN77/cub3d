/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_cfg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:14:58 by jorexpos          #+#    #+#             */
/*   Updated: 2025/08/12 09:14:59 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

static int	parse_rgb(const char *s)
{
	char	**parts;
	int		r;
	int		g;
	int		b;

	parts = ft_split((char *)s, ',');
	if (!parts || ft_strarr_len(parts) != 3)
		error_exit("Invalid colour. Format must be RGB");
	r = ft_atoi(parts[0]);
	g = ft_atoi(parts[1]);
	b = ft_atoi(parts[2]);
	ft_strarr_free(parts);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		error_exit("Colour components outside of 0-255 allowed range");
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
		error_exit("Unknown config input");
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

void	parse_config(int fd, t_config *cfg)
{
	int		count;
	char	*line;

	count = 0;
	while (count < 6)
	{
		line = read_next_nonempty(fd);
		if (!line)
			error_exit("Missing config lines/info");
		parse_line_config(line, cfg, &count);
		free(line);
	}
}
