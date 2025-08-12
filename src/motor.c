/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:09:58 by antofern          #+#    #+#             */
/*   Updated: 2025/08/07 15:01:15 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

static void	clear_background(t_world *world)
{
	int		x;
	int		y;

	y = 0;
	while (y < WINDOW_HEIGHT / 2)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			ft_pixel_put(world->data, x, y, world->textures.ceiling_color);
			x++;
		}
		y++;
	}
	y = WINDOW_HEIGHT / 2;
	while (y < WINDOW_HEIGHT)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			ft_pixel_put(world->data, x, y, world->textures.floor_color);
			x++;
		}
		y++;
	}
}

static double	one_ray(int i, t_wall *wall,
	t_world *world, double *impact_on_wall)
{
	t_ray		ray;
	double		plane_portion;
	const char	**map = (const char **)world->map;

	plane_portion = i * 2.0 / WINDOW_WIDTH - 1.0;
	ray.ray_dir.x = world->char_direction.x
		+ world->plane_direction.x * plane_portion;
	ray.ray_dir.y = world->char_direction.y
		+ world->plane_direction.y * plane_portion;
	init_ray(&(world->char_position), &(ray.ray_dir), &(ray));
	while (1)
	{
		if (ray.tile_x < 0 || ray.tile_y < 0 || ray.tile_x >= (world->map_width)
			|| ray.tile_y >= (world->map_height))
		{
			*wall = ERROR;
			return (7470000.747);
		}
		if (map[ray.tile_y][ray.tile_x] == '1')
		{
			return (hit_on_wall(&ray, world, wall, impact_on_wall));
		}
		go_further(&ray, wall);
	}
}

static void	draw_image(t_world *world)
{
	int			i;
	t_column	column;

	ft_bzero(&column, sizeof(t_column));
	clear_background(world);
	i = 0;
	while (i < WINDOW_WIDTH)
	{
		column.distance = one_ray(i, &(column.wall), world, &(column.impact));
		print_one_column(world, i, &column);
		i++;
	}
}

int	motor(t_world *world)
{
	t_data	*data;

	data = world->data;
	char_movement(world);
	draw_image(world);
	draw_minimap(world);
	mlx_put_image_to_window(data->mlx, data->window, data->img, 0, 0);
	return (1);
}
