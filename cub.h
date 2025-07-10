/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 13:31:00 by antofern          #+#    #+#             */
/*   Updated: 2025/07/10 20:41:05 by antofern         ###   ########.fr       */
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
# define MAX_RAY_DISTANCE 1e30
# define PLAYER_ROTATION_STEP 0.06 
//Redefinimos macros de X.h con otro nombre, por ambiguedad en La Norma
	// Eventos
#define KEY_PRESS        2
#define KEY_RELEASE      3
#define DESTROY_NOTIFY   17
	// Máscaras de eventos
#define KEY_PRESS_MASK    (1L<<0)
#define KEY_RELEASE_MASK  (1L<<1)

# define M_PI 3.14159265358979323846

typedef enum e_wall
{
	VERTICAL,
	HORIZONTAL,
} t_wall;

typedef struct s_vector
{
    double x;
    double y;
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

typedef struct s_ray
{
	int 	tile_x; //casilla actual durante el calculo (no es el origen)
	double	side_dist_x;
	double	delta_dist_x;
	int		step_x;

	int		tile_y;
	double	side_dist_y;
	double	delta_dist_y;
	int		step_y;

	t_wall wall;

	double ray_len;


}	t_ray;

/*
typedef struct s_frame
{
	t_data		*data;
	// mapa, plano cartesiano etc
}		t_frame;
*/
typedef struct s_textures
{
	int	floor_color;
	int	 ceiling_color;
	char *no_texture;
	char *so_texture;
	char *we_texture;
	char *ea_texture;
}	t_textures;

typedef struct s_world
{
	t_textures textures;
	
	//una funcion debe sustituir N S E o W por un cero, despues de anotar posicion y direccion del jugador en el struct world.
	char **map; 
	int map_height; //Medidas tomadas despues de rectangulizar el mapa
	int map_width;
	t_vector template_vectors[WINDOW_WIDTH];

	int		key;
	t_vector char_position;
	t_vector char_direction;
	t_data *data;
}		t_world;

//utils_for_mlx.c
void	ft_pixel_put(t_data *data, int x, int y, int color);
int		press_key(int keycode, t_world *world);
int		release_key(int keycode, t_world *world);
int		free_data(t_data **data, int error_code);
int		close_win(t_world *world);
int		init_data(t_data **data);

//render.c
int	motor(t_world *world);

//+++++++++++++++++++++++++++++++++++//
// BORRAR ANTES DE ENTREGAR MOOKS    //
//+++++++++++++++++++++++++++++++++++//
#include <pthread.h>
typedef struct s_map_data
{
    t_world *world;
    int     tile_size;
	t_data	data;
} t_map_data;
	//MOOKS.c (funciones de prueba y debugging)
void rotate_direction(t_vector *direction, double angle);
void draw_a_square(t_data *img);
void *map_thread(void *arg);
void render_map(t_map_data *map_data);
int update_map(void *param);
void draw_map(t_world *world);



#endif