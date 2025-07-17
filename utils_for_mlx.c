/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_mlx.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 16:59:20 by antofern          #+#    #+#             */
/*   Updated: 2025/07/17 17:14:17 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

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
	{
		close_win(world);
	}
	else
	{
		world->key = keycode;
	}
	return (0);
}

int	release_key(int keycode, t_world *world)
{
	if (world->key == keycode)
		world->key = 0;
	return (0);
}

int	free_data(t_data **data, int error_code)
{
	if (error_code == 1)
		perror("mlx_init() fail in init_data");
	if (error_code == 2)
		perror("mlx_new_window() fail in init_data");
	if (error_code == 3)
		perror("mlx_new_image() fail in init_data");
	if (error_code == 4)
		perror("mlx_get_data_addr() fail in init_data");
	if (*data && (*data)->img)
		mlx_destroy_image((*data)->mlx, (*data)->img);
	if (*data && (*data)->mlx && (*data)->window)
		mlx_destroy_window((*data)->mlx, (*data)->window);
	if (*data && (*data)->mlx)
	{
		mlx_destroy_display((*data)->mlx);
		free((*data)->mlx);
	}
	if (*data)
		free(*data);
	return (error_code);
}

int	close_win(t_world *world)
{
	free_data(&(world->data), CLOSING_WINDOW);
	exit(EXIT_SUCCESS);
}

int	init_data(t_data **data)
{
	*data = (t_data *)malloc(sizeof(t_data));
	if (*data == NULL)
	{
		perror("malloc fail in init_data");
		exit (1);
	}
	(*data)->window = NULL;
	(*data)->img = NULL;
	(*data)->mlx = mlx_init();
	if ((*data)->mlx == NULL)
		exit (free_data(data, ERROR_MLX_INIT));
	(*data)->window = mlx_new_window((*data)->mlx, WINDOW_WIDTH, WINDOW_HEIGHT,
			"cub3D");
	if ((*data)->window == NULL)
		exit (free_data(data, ERROR_MLX_NEW_WINDOW));
	(*data)->img = mlx_new_image((*data)->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if ((*data)->img == NULL)
		exit (free_data(data, ERROR_MLX_NEW_IMAGE));
	(*data)->addr = mlx_get_data_addr((*data)->img, &(*data)->bits_per_pixel,
			&(*data)->line_length, &(*data)->endian);
	if ((*data)->addr == NULL)
		exit (free_data(data, ERROR_MLX_GET_ADDR));
	return (0);
}
