/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 13:31:00 by antofern          #+#    #+#             */
/*   Updated: 2025/07/17 12:03:45 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H

# include "minilibx-linux/mlx.h"
# include <math.h>
# include "libft/libft.h"
#include <stdio.h>

//Codigos de error para MLX
# define ERROR_MLX_INIT 1
# define ERROR_MLX_NEW_WINDOW 2
# define ERROR_MLX_NEW_IMAGE 3
# define ERROR_MLX_GET_ADDR 4
# define CLOSING_WINDOW 0

//Definiciones de constantes
# define PLANE_MAGNITUDE 0.577 //PLANE_MAGNITUDE = tan(FOV / 2) = tan(π/6) ≈ 0.577
# define WINDOW_WIDTH 640
# define WINDOW_HEIGHT 480
# define MAX_RAY_DISTANCE 1e30
# define PLAYER_ROTATION_STEP 0.001//
# define PLAYER_MOVE_STEP 0.1
# define M_PI 3.14159265358979323846 //Pi por algun motivo no se carga desde math.h
# define TEXTURE_WIDTH 64
# define TEXTURE_HEIGHT 64

//Redefinimos macros de X.h con otro nombre, por ambiguedad en La Norma
	// Eventos
#define KEY_PRESS        2
#define KEY_RELEASE      3
#define DESTROY_NOTIFY   17
	// Máscaras de eventos
#define KEY_PRESS_MASK    (1L<<0)
#define KEY_RELEASE_MASK  (1L<<1)

//Para indicar en que tipo de pared golpeó un rayo
typedef enum e_wall
{
	NORTH,
	SOUTH,
	WEST,
	EAST,
	//ignora estos:
	VERTICAL,
	HORIZONTAL,
	ERROR
} t_wall;

typedef struct s_vector
{
	double x;
	double y;
} t_vector;

//Estructura de datos de la ventana y la imagen (mlx)
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
	t_vector ray_dir;
	int 	tile_x; //casilla actual durante el calculo (no es el origen)
	int		tile_y;
	double	side_dist_x;// Distancia al siguiente muro en x
	double	side_dist_y;
	double	delta_dist_x; //distancia entre dos lineas del mismo eje
	double	delta_dist_y;
	int		step_x; // +1 o -1, nos movemos hacia delante(1) o hacia detras(-1) en x
	int		step_y;
}	t_ray;

typedef struct s_textures
{
	int wall_color; //BORRAR es de prueba!!
	int	floor_color;
	int	 ceiling_color;
	char *no_texture;
	char *so_texture;
	char *we_texture;
	char *ea_texture;
}	t_textures;

typedef struct s_world
{
	t_data		*data; //puntero a la estructura de datos de la ventana y la imagen
	t_textures	textures;
	char		**map; 
	int			map_height; //Medidas tomadas despues de rectangulizar el mapa
	int			map_width;
	int			key; //Ultima tecla pulsada
	t_vector	char_position;
	t_vector	char_direction;
	t_vector	plane_direction; //perpendicular a char_direction, para calcular el vector de vision
}		t_world;

//char_movement.c
void	char_movement(t_world *world);
void	rotate_vector(t_vector *vector, double sin, double cos);

//main.c
void	calculate_camera_plane(double char_dir_x, double char_dir_y, t_vector *plane_direction);
int	init_world(t_world *world, t_data *data);

//utils_for_mlx.c
void	ft_pixel_put(t_data *data, int x, int y, int color);
int		press_key(int keycode, t_world *world);
int		release_key(int keycode, t_world *world);
int		free_data(t_data **data, int error_code);
int		close_win(t_world *world);
int		init_data(t_data **data);

//render.c
int		motor(t_world *world);
double	one_ray(int i, t_wall *wall, t_world *world, double *impact_on_wall);
void	draw_image(t_world *world);
void	init_ray(t_vector *char_position, const t_vector *vector, t_ray *ray);
void	calculate_distances(t_world *world, const t_vector vectors[], double distances[], t_wall *wall);
void	print_columns(double distances[], t_data *data, t_textures textures);
void	calc_side_dist(t_ray *ray, t_vector *char_position, const t_vector *vector);

//+++++++++++++++++++++++++++++++++++//
// BORRAR ANTES DE ENTREGAR MOOKS    //
//+++++++++++++++++++++++++++++++++++//
#define DEBUGMODE 0 //1 para activar mensajes de debug, 0 para desactivarlos
#define DEBUGMODE2 0
//+++++++++++++++++++++++++++++++++++//

#endif