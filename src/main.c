/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:14:27 by jorexpos          #+#    #+#             */
/*   Updated: 2025/08/12 09:14:29 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

static void	calculate_camera_plane(double char_dir_x, double char_dir_y,
	t_vector *plane_direction)
{
	plane_direction->x = -char_dir_y * PLANE_MAGNITUDE;
	plane_direction->y = char_dir_x * PLANE_MAGNITUDE;
}

static void	set_player(t_world *w, size_t i, size_t j, char c)
{
	w->char_position.x = j + 0.5;
	w->char_position.y = i + 0.5;
	if (c == 'N')
		w->char_direction = (t_vector){0, -1};
	else if (c == 'S')
		w->char_direction = (t_vector){0, 1};
	else if (c == 'E')
		w->char_direction = (t_vector){1, 0};
	else if (c == 'W')
		w->char_direction = (t_vector){-1, 0};
	calculate_camera_plane(w->char_direction.x, w->char_direction.y,
		&w->plane_direction);
	w->map[i][j] = '0';
}

static int	init_world_player(t_world *w)
{
	size_t	i;
	size_t	j;
	char	c;

	i = 0;
	while (i < (size_t)w->map_height)
	{
		j = 0;
		while (j < ft_strlen(w->map[i]))
		{
			c = w->map[i][j];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				set_player(w, i, j, c);
				return (0);
			}
			j++;
		}
		i++;
	}
	return (-1);
}

static int	init_world(t_world *world, t_data *data, t_config *cfg)
{
	size_t	i;
	size_t	len;
	size_t	max_width;

	world->data = data;
	world->textures.no_texture = cfg->tex_no;
	world->textures.so_texture = cfg->tex_so;
	world->textures.we_texture = cfg->tex_we;
	world->textures.ea_texture = cfg->tex_ea;
	world->textures.floor_color = cfg->floor_color;
	world->textures.ceiling_color = cfg->ceil_color;
	world->map = cfg->map;
	i = 0;
	max_width = 0;
	while (cfg->map[i])
	{
		len = ft_strlen(cfg->map[i]);
		if (len > max_width)
			max_width = len;
		i++;
	}
	world->map_width = (int)max_width;
	world->map_height = (int)i;
	return (init_world_player(world));
}

int	main(int argc, char **argv)
{
	t_config	config;
	t_world		world;
	t_data		*data;
	int			i;

	ft_bzero(&config, sizeof(t_config));
	if (argc != 2)
		error_exit("Use: ./cub3D <map_file.cub>");
	check_extension(argv[1]);
	parse_map(argv[1], &config);
	init_data(&data);
	init_world(&world, data, &config);
	load_textures(&world);
	i = 0;
	while (i < 256)
		world.key_down[i++] = false;
	mlx_hook(data->window, KEY_PRESS, 1L << 0, press_key, &world);
	mlx_hook(data->window, KEY_RELEASE, 1L << 1, release_key, &world);
	mlx_hook(data->window, DESTROY_NOTIFY, 0, close_win, &world);
	mlx_loop_hook(data->mlx, motor, &world);
	mlx_loop(data->mlx);
	return (0);
}
