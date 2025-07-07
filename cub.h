/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 13:31:00 by antofern          #+#    #+#             */
/*   Updated: 2025/07/07 20:16:32 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H

# include "minilibx-linux/mlx.h"
# include <math.h>
# include "libft/libft.h"
#include <stdio.h>

# define ERROR_MLX_INIT 1
# define ERROR_MLX_NEW_WINDOW 2
# define ERROR_MLX_NEW_IMAGE 3
# define ERROR_MLX_GET_ADDR 4
# define CLOSING_WINDOW 0
# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 800

typedef struct s_point
{
    double x;
    double y;
} t_point;

typedef struct s_vector
{
    double cos;
    double sin;
} t_vector;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	void	*mlx;
	void	*window;

}				t_data;

/*
typedef struct s_frame
{
	t_data		*data;
	// mapa, plano cartesiano etc
}		t_frame;
*/

typedef struct s_world
{
	int floor_color;
	int ceiling_color;

	char *no_texture;
	char *so_texture;
	char *we_texture;
	char *ea_texture;

	//una funcion debe sustituir N S E o W por un cero, despues de anotar posicion y direccion del jugador en el struct world.
	char **map; 
	int map_height; //Medidas tomadas despues de rectangulizar el mapa
	int map_width;


	t_point char_position;
	t_vector char_direction;
	t_data *data;
}		t_world;

//utils_for_mlx.c
void	ft_pixel_put(t_data *data, int x, int y, int color);
int		manage_key(int keycode, t_world *world);
int		free_data(t_data **data, int error_code);
int		close_win(t_world *world);
int		init_data(t_data **data);



#endif