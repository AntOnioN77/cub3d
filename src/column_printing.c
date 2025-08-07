/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   column_printing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:32:15 by antofern          #+#    #+#             */
/*   Updated: 2025/07/29 13:21:23 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

static char	*get_texture_addr(t_textures *tex, t_wall wall)
{
	if (wall == NORTH)
		return (tex->no_addr);
	if (wall == SOUTH)
		return (tex->so_addr);
	if (wall == WEST)
		return (tex->we_addr);
	if (wall == EAST)
		return (tex->ea_addr);
	return (NULL);
}

static void	calculate_draw_limits(double distance, int *start, int *end)
{
	int	line_h;

	line_h = (int)(WINDOW_HEIGHT / distance);
	*start = (WINDOW_HEIGHT - line_h) / 2;
	*end = *start + line_h;
	if (*start < 0)
		*start = 0;
	if (*end > WINDOW_HEIGHT)
		*end = WINDOW_HEIGHT;
}

static void	draw_pixel_column(t_draw_info *info, int start, int end)
{
	int		y;
	int		d;
	int		tex_y;
	char	*pixel;

	y = start;
	while (y < end)
	{
		d = y * 256 - WINDOW_HEIGHT * 128 + info->line_h * 128;
		tex_y = ((d * info->tex->height) / info->line_h) / 256;
		pixel = info->tex_addr + (tex_y * info->tex->line_length
				+ info->tex_x * (info->tex->bpp / 8));
		ft_pixel_put(info->data, info->i, y, *(unsigned int *)pixel);
		y++;
	}
}

void	print_one_column(t_world *world, int i, t_column *column)
{
	t_draw_info			info;
	int					limits[2];
	double				distance;
	static const double	min_texture_distance = TEXTUR_MIN_RATIO * WINDOW_HEIGHT;
	static const double	max_texture_distance = TEXTUR_MAX_RATIO * WINDOW_HEIGHT;

	distance = column->distance;
	calculate_draw_limits(distance, &limits[0], &limits[1]);
	info.data = world->data;
	info.tex = &world->textures;
	info.tex_addr = get_texture_addr(info.tex, column->wall);
	info.tex_x = (int)column->impact % info.tex->width;
	if (distance < min_texture_distance)
		distance = min_texture_distance;
	if (distance > max_texture_distance)
		distance = max_texture_distance;
	info.line_h = (int)((double)WINDOW_HEIGHT / distance);
	info.i = i;
	draw_pixel_column(&info, limits[0], limits[1]);
}
