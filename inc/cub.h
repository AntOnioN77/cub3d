/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 13:31:00 by antofern          #+#    #+#             */
/*   Updated: 2025/08/07 15:16:17 by antofern         ###   ########.fr       */
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

# define ERROR_MLX_INIT 1
# define ERROR_MLX_NEW_WINDOW 2
# define ERROR_MLX_NEW_IMAGE 3
# define ERROR_MLX_GET_ADDR 4
# define CLOSING_WINDOW 0

//PLANE_MAGNITUDE = tan(FOV / 2) = tan(π/6) ≈ 0.577
# define PLANE_MAGNITUDE 0.577
# define WINDOW_WIDTH 1280//640
# define WINDOW_HEIGHT 720//480
# define TEXTUR_MIN_RATIO 0.000005
# define TEXTUR_MAX_RATIO 0.03

# define MAX_RAY_DISTANCE 1e30
// sin(0.03)
# define ROT_SIN 0.0299955002
// cos(0.03)
# define ROT_COS 0.99955003374
# define STEP 0.03
//π
# define M_PI 3.14159265358979323846
# define TEXTURE_WIDTH 64
# define TEXTURE_HEIGHT 64
# define PLAYER_RADIUS 0.1

# define ERR_PREFIX "Error\n"
# define ERR_PREFIX "Error\n"
# define KEY_PRESS        2
# define KEY_RELEASE      3
# define KEY_W 44
# define KEY_A 97
# define KEY_S 111
# define KEY_D 101
# define DESTROY_NOTIFY   17

typedef enum e_wall
{
	NORTH,
	SOUTH,
	WEST,
	EAST,
	VERTICAL,
	HORIZONTAL,
	ERROR
}	t_wall;

typedef struct s_vector
{
	double	x;
	double	y;
}	t_vector;

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
	int			tile_x;
	int			tile_y;
	double		side_dist_x;
	double		side_dist_y;
	double		delta_dist_x;
	double		delta_dist_y;
	int			step_x;
	int			step_y;
}	t_ray;

typedef struct s_minimap
{
	int	offset_x;
	int	offset_y;
	int	view_w;
	int	view_h;
}	t_minimap;

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

typedef struct s_draw_info
{
	t_data		*data;
	t_textures	*tex;
	char		*tex_addr;
	int			tex_x;
	int			line_h;
	int			i;
}	t_draw_info;

typedef struct s_world
{
	t_data		*data;
	t_textures	textures;
	char		**map;
	int			map_height;
	int			map_width;
	int			key;
	t_vector	char_position;
	t_vector	char_direction;
	t_vector	plane_direction;
	bool		key_down[256];
}		t_world;

typedef struct s_column
{
	double		distance;
	t_wall		wall;
	double		impact;
}	t_column;

//char_movement.c

void	char_movement(t_world *world);
void	rotate_vector(t_vector *vector, double sinv, double cosv);
void	set_position(double new_position_x, double new_position_y,
			t_world *world);

//utils_for_mlx.c
void	ft_pixel_put(t_data *data, int x, int y, int color);
int		press_key(int keycode, t_world *world);
int		release_key(int keycode, t_world *world);

//load_textures.c
void	load_textures(t_world *w);

//memory_handler.c
int		close_win(t_world *world);
int		init_data(t_data **data);

//motor.c
void	set_wall_type(t_wall *wall, t_vector *vector);
int		motor(t_world *world);
void	calculate_distances(t_world *world, const t_vector vectors[],
			double distances[], t_wall *wall);
void	print_columns(double distances[], t_data *data, t_textures textures);

//motor_utils.c
void	init_ray(t_vector *char_position, const t_vector *vector, t_ray *ray);
double	hit_on_wall(t_ray *ray, t_world *world, t_wall *wall,
			double *impact_on);
double	calc_impact_on_wall(t_vector *ray_dir, t_wall wall,
			t_world *world, double distance);
void	go_further(t_ray *ray, t_wall *wall);
void	calc_side_dist(t_ray *ray, t_vector *char_position,
			const t_vector *vector);

//map_check.c
int		has_valid_chars(char **map);
int		is_map_closed(char **map);
int		has_one_player(char **map);

//parse_map.c
void	check_extension(const char *file);
void	parse_map(const char *path, t_config *cfg);

//parse_map_cfg.c
void	parse_config(int fd, t_config *cfg);

//map_check_utils.c
void	error_exit(const char *msg);
size_t	ft_strarr_len(char **arr);
void	ft_strarr_free(char **arr);
char	**ft_strarr_append(char **arr, const char *new_str);

//normalize_map.c
char	**normalize_map(char **map);

//minimap.c
void	draw_minimap(t_world *world);

//column_printing.c
void	print_one_column(t_world *world, int i, t_column *column);

#endif