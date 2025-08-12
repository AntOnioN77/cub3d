/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:14:46 by jorexpos          #+#    #+#             */
/*   Updated: 2025/08/12 09:14:47 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

static void	free_textures(t_data *data, t_textures *tex)
{
	if (tex->no_img)
		mlx_destroy_image(data->mlx, tex->no_img);
	if (tex->so_img)
		mlx_destroy_image(data->mlx, tex->so_img);
	if (tex->we_img)
		mlx_destroy_image(data->mlx, tex->we_img);
	if (tex->ea_img)
		mlx_destroy_image(data->mlx, tex->ea_img);
}

static int	free_data(t_data **data, int error_code)
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
	free_textures(world->data, &world->textures);
	if (world->textures.no_texture)
		free(world->textures.no_texture);
	if (world->textures.so_texture)
		free(world->textures.so_texture);
	if (world->textures.we_texture)
		free(world->textures.we_texture);
	if (world->textures.ea_texture)
		free(world->textures.ea_texture);
	ft_strarr_free(world->map);
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
