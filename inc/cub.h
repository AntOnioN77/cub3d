/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 13:31:00 by antofern          #+#    #+#             */
/*   Updated: 2025/07/29 12:44:52 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H

# include "../minilibx-linux/mlx.h"
# include <math.h>
# include "../libft/libft.h"
# include "../get_next_line/get_next_line.h"
# include <stdio.h>
# include <stdbool.h>
# include <fcntl.h>
# include <errno.h>

//Codigos de error para MLX
# define ERROR_MLX_INIT 1
# define ERROR_MLX_NEW_WINDOW 2
# define ERROR_MLX_NEW_IMAGE 3
# define ERROR_MLX_GET_ADDR 4
# define CLOSING_WINDOW 0

//Definiciones de constantes
	//PLANE_MAGNITUDE = tan(FOV / 2) = tan(π/6) ≈ 0.577
# define PLANE_MAGNITUDE 0.577
# define WINDOW_WIDTH 640
# define WINDOW_HEIGHT 480
# define TEXTUR_MIN_RATIO 0.000005
# define TEXTUR_MAX_RATIO 0.03

# define MAX_RAY_DISTANCE 1e30
# define ROT_STEP 0.01
# define ROT_SIN 0.009999833334166664   // sin(0.01)
# define ROT_COS 0.9999500004166653     // cos(0.01)
# define MOVE_STEP 0.01
# define M_PI 3.14159265358979323846 //Pi por algun motivo no se carga desde math.h
# define TEXTURE_WIDTH 64
# define TEXTURE_HEIGHT 64

//Definición de macro de error para el parseo:
# define ERR_PREFIX "Error\n"

//Redefinimos macros de X.h con otro nombre, por ambiguedad en La Norma
	// Eventos
# define KEY_PRESS        2
# define KEY_RELEASE      3
# define DESTROY_NOTIFY   17
	// Máscaras de eventos
# define KEY_PRESS_MASK    (1L<<0)
# define KEY_RELEASE_MASK  (1L<<1)

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
}	t_wall;

typedef struct s_vector
{
	double x;
	double y;
}	t_vector;

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
	t_vector	ray_dir;
	int			tile_x;//casilla actual durante el calculo (no es el origen)
	int			tile_y;
	double		side_dist_x;// Distancia al siguiente muro en x
	double		side_dist_y;
	double		delta_dist_x; //distancia entre dos lineas del mismo eje
	double		delta_dist_y;
	int			step_x; // +1 o -1, nos movemos hacia delante(1) o hacia detras(-1) en x
	int			step_y;
}	t_ray;

// Configuración para el parseo
typedef struct s_config
{
	char	*tex_no;
	char	*tex_so;
	char	*tex_we;
	char	*tex_ea;
	int		floor_color;
	int		ceil_color;
	char	**map;
}				t_config;

typedef struct s_textures
{
	char	*no_texture;
	char	*so_texture;
	char	*we_texture;
	char	*ea_texture;
	int		ceiling_color;
	int		floor_color;
	void	*no_img;
	void	*so_img;
	void	*we_img;
	void	*ea_img;
	char	*no_addr;
	char	*so_addr;
	char	*we_addr;
	char	*ea_addr;
	int		width;
	int		height;
	int		bpp;
	int		line_length;
	int		endian;
}	t_textures;

// Para poder dibujar las texturas
typedef struct s_draw_info
{
    t_data      *data;
    t_textures  *tex;
    char        *tex_addr;
    int         tex_x;
    int         line_h;
    int         i;
}	t_draw_info;

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
	bool    key_down[256]; // Para poder movernos en varias direcciones a la vez
}		t_world;

typedef struct s_column
{
    double distance;
    t_wall wall;
    double impact;
} t_column;

//char_movement.c

void	char_movement(t_world *world);
void	rotate_vector(t_vector *vector, double sinv, double cosv);

//main.c
void	calculate_camera_plane(double char_dir_x, double char_dir_y, t_vector *plane_direction);
int		init_world(t_world *world, t_data *data, t_config *cfg);

//utils_for_mlx.c
void	ft_pixel_put(t_data *data, int x, int y, int color);
int		press_key(int keycode, t_world *world);
int		release_key(int keycode, t_world *world);
int		free_data(t_data **data, int error_code);
int		close_win(t_world *world);
int		init_data(t_data **data);

//motor.c
double	hit_on_wall(t_ray *ray, t_world *world, t_wall *wall,
			double *impact_on_wall);
double	calc_impact_on_wall(t_vector *ray_dir, t_wall wall,
			t_world *world, double distance);
void	go_further(t_ray *ray, t_wall *wall);
void	set_wall_type(t_wall *wall, t_vector *vector);
int		motor(t_world *world);
double	one_ray(int i, t_wall *wall, t_world *world, double *impact_on_wall);
void	draw_image(t_world *world);
void	init_ray(t_vector *char_position, const t_vector *vector, t_ray *ray);
void	calculate_distances(t_world *world, const t_vector vectors[], double distances[], t_wall *wall);
void	print_columns(double distances[], t_data *data, t_textures textures);
void	calc_side_dist(t_ray *ray, t_vector *char_position, const t_vector *vector);

//map_check.c
void	error_exit(const char *msg);
char	**collect_map(int fd);
void	parse_file(const char *path, t_config *cfg);
char	**ft_strarr_append(char **arr, const char *new_str);
void	check_extension(const char *file);

//column_printing.c
void	print_one_column(t_world *world, int i, t_column *column);

#endif