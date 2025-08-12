/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_mlx.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 16:59:20 by antofern          #+#    #+#             */
/*   Updated: 2025/08/12 09:15:07 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

void	ft_pixel_put(t_data *data, int x, int y, int color)
{
	char	*pixel;

	pixel = data->addr + (y * data->line_length + x
			* (data->bits_per_pixel / 8));
	*(unsigned int *)pixel = color;
}

int	press_key(int keycode, t_world *world)
{
	if (keycode == 65307)
		close_win(world);
	else if (keycode >= 0 && keycode < 256)
		world->key_down[keycode] = true;
	return (0);
}

int	release_key(int keycode, t_world *world)
{
	if (keycode >= 0 && keycode < 256)
		world->key_down[keycode] = false;
	return (0);
}
