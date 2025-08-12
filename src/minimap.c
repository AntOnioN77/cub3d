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

static void	draw_map_tiles(t_world *world, int offset_x, int offset_y)
{
	int	map_x;
	int	map_y;
	int	color;

	map_y = 0;
	while (map_y < world->map_height)
	{
		map_x = 0;
		while (map_x < world->map_width)
		{
			if (world->map[map_y][map_x] == '1')
				color = 0x000000;
			else
				color = 0xFFFFFF;
			draw_square(world->data,
				offset_x + map_x * 8,
				offset_y + map_y * 8,
				color);
			map_x++;
		}
		map_y++;
	}
}

static void	draw_player(t_world *world, int offset_x, int offset_y)
{
	int	px;
	int	py;

	px = offset_x + (int)(world->char_position.x * 8);
	py = offset_y + (int)(world->char_position.y * 8);
	draw_square(world->data, px, py, 0xFF0000);
}

void	draw_minimap(t_world *world)
{
	int	offset_x;
	int	offset_y;

	offset_x = 10;
	offset_y = 10;
	draw_map_tiles(world, offset_x, offset_y);
	draw_player(world, offset_x, offset_y);
}
