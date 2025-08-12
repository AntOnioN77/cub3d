#include "../inc/cub.h"

void load_textures(t_world *w)
{
	t_data *data = w->data;

	w->textures.no_img = mlx_xpm_file_to_image(
		data->mlx, w->textures.no_texture, 
		&w->textures.width, &w->textures.height);
	if (!w->textures.no_img)
		error_exit("Error: textura norte no encontrada");
	w->textures.so_img = mlx_xpm_file_to_image(
		data->mlx, w->textures.so_texture, 
		&w->textures.width, &w->textures.height);
	if (!w->textures.so_img)
		error_exit("Error: textura sur no encontrada");
	w->textures.we_img = mlx_xpm_file_to_image(
		data->mlx, w->textures.we_texture, 
		&w->textures.width, &w->textures.height);
	if (!w->textures.we_img)
		error_exit("Error: textura oeste no encontrada");
	w->textures.ea_img = mlx_xpm_file_to_image(
		data->mlx, w->textures.ea_texture, 
		&w->textures.width, &w->textures.height);
	if (!w->textures.ea_img)
		error_exit("Error: textura este no encontrada");
	w->textures.no_addr = mlx_get_data_addr(w->textures.no_img,
		&w->textures.bpp, &w->textures.line_length, &w->textures.endian);
	w->textures.so_addr = mlx_get_data_addr(w->textures.so_img,
		&w->textures.bpp, &w->textures.line_length, &w->textures.endian);
	w->textures.we_addr = mlx_get_data_addr(w->textures.we_img,
		&w->textures.bpp, &w->textures.line_length, &w->textures.endian);
	w->textures.ea_addr = mlx_get_data_addr(w->textures.ea_img,
		&w->textures.bpp, &w->textures.line_length, &w->textures.endian);
}

/* Busca al jugador y configura dirección y plano de cámara */
static int	init_world_player(t_world *world)
{
	size_t	i;
	size_t	j;
	char	c;

	i = 0;
	while (i < (size_t)world->map_height)
	{
		j = 0;
		while (j < ft_strlen(world->map[i]))
		{
			c = world->map[i][j];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				world->char_position.x = j + 0.5;
				world->char_position.y = i + 0.5;
				if (c == 'N')
					world->char_direction = (t_vector){0, -1};
				else if (c == 'S')
					world->char_direction = (t_vector){0, 1};
				else if (c == 'E')
					world->char_direction = (t_vector){1, 0};
				else if (c == 'W')
					world->char_direction = (t_vector){-1, 0};
				calculate_camera_plane(world->char_direction.x,
					world->char_direction.y, &world->plane_direction);
				world->map[i][j] = '0';
				return (0);
			}
			j++;
		}
		i++;
	}
	return (-1);
}

int	init_world(t_world *world, t_data *data, t_config *cfg)
{
	size_t	i;
	size_t	len;
	size_t	max_width;

	world->data = data;
	world->textures.no_texture = cfg->tex_no;
	world->textures.so_texture = cfg->tex_so;
	world->textures.we_texture = cfg->tex_we;
	world->textures.ea_texture = cfg->tex_ea;
	world->textures.floor_color = cfg->floor_color;
	world->textures.ceiling_color = cfg->ceil_color;
	world->map = cfg->map;
	i = 0;
	max_width = 0;
	while (cfg->map[i])
	{
		len = ft_strlen(cfg->map[i]);
		if (len > max_width)
			max_width = len;
		i++;
	}
	world->map_width = (int)max_width;
	world->map_height = (int)i;
	return (init_world_player(world));
}

/**
 * calculate_camera_plane - Calcula el vector del plano de proyección de la cámara
 * 
 * @char_dir_x: Componente X del vector dirección normalizado del jugador.
 * @char_dir_y: Componente Y del vector dirección normalizado del jugador.
 * @plane_direction: Puntero donde se almacenará el vector del plano de cámara calculado.
 * 
 * Descripción:
 * Genera un vector perpendicular al vector dirección del jugador que define el
 * plano de proyección de la cámara. Este plano es fundamental para el raycasting:
 * determina el campo de visión (FOV) y se usa para calcular la dirección de cada
 * rayo. El vector se obtiene rotando 90 grados el vector dirección (intercambiando
 * componentes y negando uno) y escalándolo por PLANE_MAGNITUDE. La magnitud del
 * plano controla directamente el FOV: mayor magnitud = mayor FOV. Un valor de 0.66
 * da aproximadamente 66° de FOV.
 * 
 * Retorno:
 * - void
 * 
 * Observaciones:
 * - La rotación de 90° se logra con la transformación: (x,y) → (-y,x).
 * - PLANE_MAGNITUDE típicamente vale 0.66 para un FOV realista.
 * - El plano debe ser perpendicular a la dirección para evitar distorsión.
 * - Se recalcula cada vez que el jugador rota (en char_movement).
 * - Un plano más largo simula una lente gran angular; más corto simula teleobjetivo.
 * - Este vector define la "anchura" del campo visual en el espacio del mundo.
 * 
 * Funciones llamadas:
 * - Ninguna
 */
void	calculate_camera_plane(double char_dir_x, double char_dir_y,
	t_vector *plane_direction)
{
	plane_direction->x = -char_dir_y * PLANE_MAGNITUDE;
	plane_direction->y = char_dir_x * PLANE_MAGNITUDE;
}

int	main(int argc, char **argv)
{
	t_config	config;
	t_world		world;
	t_data		*data;
	int			i;

	ft_bzero(&config, sizeof(t_config));
	if (argc != 2)
		error_exit("Uso: ./cub3D <archivo_de_config>");
	check_extension(argv[1]);
	parse_map(argv[1], &config);
	init_data(&data);
	init_world(&world, data, &config);
	load_textures(&world);
	i = 0;
	while (i < 256)
		world.key_down[i++] = false;
	mlx_hook(data->window, KEY_PRESS, KEY_PRESS_MASK, press_key, &world);
	mlx_hook(data->window, KEY_RELEASE, KEY_RELEASE_MASK, release_key, &world);
	mlx_hook(data->window, DESTROY_NOTIFY, 0, close_win, &world);
	mlx_loop_hook(data->mlx, motor, &world);
	mlx_loop(data->mlx);
	return (0);
}
