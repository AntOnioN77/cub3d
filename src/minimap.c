/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:43:32 by jorexpos          #+#    #+#             */
/*   Updated: 2025/08/12 13:43:33 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

static void	draw_square(t_data *data, int x, int y, int color)
{
	int	dx;
	int	dy;

	dy = 0;
	while (dy < 8)
	{
		dx = 0;
		while (dx < 8)
		{
			ft_pixel_put(data, x + dx, y + dy, color);
			dx++;
		}
		dy++;
	}
}

static int	get_tile_color(t_world *world, int map_x, int map_y)
{
	if (map_y >= 0 && map_y < world->map_height
		&& map_x >= 0 && map_x < world->map_width)
	{
		if (world->map[map_y][map_x] == '1')
			return (0x000000);
		return (world->textures.floor_color);
	}
	return (0x333333);
}

static void	draw_map_tiles(t_world *world, t_minimap *m)
{
	int	map_x;
	int	map_y;
	int	start_x;
	int	start_y;

	start_x = (int)world->char_position.x - m->view_w / 2;
	start_y = (int)world->char_position.y - m->view_h / 2;
	map_y = start_y;
	while (map_y < start_y + m->view_h)
	{
		map_x = start_x;
		while (map_x < start_x + m->view_w)
		{
			draw_square(world->data,
				m->offset_x + (map_x - start_x) * 8,
				m->offset_y + (map_y - start_y) * 8,
				get_tile_color(world, map_x, map_y));
			map_x++;
		}
		map_y++;
	}
}

static void	draw_player(t_world *world, t_minimap *m)
{
	int	px;
	int	py;
	int	start_x;
	int	start_y;

	start_x = (int)world->char_position.x - m->view_w / 2;
	start_y = (int)world->char_position.y - m->view_h / 2;
	if (world->map[(int)world->char_position.y]
		[(int)world->char_position.x] != '1')
	{
		px = m->offset_x + (int)((world->char_position.x - start_x) * 8);
		py = m->offset_y + (int)((world->char_position.y - start_y) * 8);
		draw_square(world->data, px, py, 0xFF0000);
	}
}

void	draw_minimap(t_world *world)
{
	t_minimap	m;

	m.view_w = 20;
	m.view_h = 20;
	m.offset_x = 1280 - m.view_w * 8 - 10;
	m.offset_y = 10;
	draw_map_tiles(world, &m);
	draw_player(world, &m);
}
