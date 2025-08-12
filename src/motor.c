/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:09:58 by antofern          #+#    #+#             */
/*   Updated: 2025/08/10 19:11:09 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

static void	clear_background(t_world *world);

/**
 * motor - Función principal del bucle de renderizado del juego
 * 
 * @world: Puntero a la estructura principal que contiene todo el estado del juego
 *         (mapa, jugador, texturas, datos de MLX).
 * 
 * Descripción:
 * Función callback ejecutada continuamente por mlx_loop_hook(). Constituye el 
 * game loop principal del motor gráfico. En cada iteración procesa el movimiento
 * del jugador, renderiza la nueva imagen 3D y la muestra en la ventana.
 * Es el corazón del motor de renderizado, coordinando input y output visual.
 * 
 * Retorno:
 * - Siempre retorna 1 (requerido por MLX para continuar el loop).
 * 
 * Observaciones:
 * - Se ejecuta aproximadamente 60 veces por segundo (depende de MLX).
 * - El orden es importante: primero movimiento, luego renderizado.
 * - No debe realizar operaciones bloqueantes para mantener fluidez.
 * 
 * Funciones llamadas:
 * - char_movement(): Procesa las teclas presionadas y actualiza la posición
 * - draw_image(): Renderiza el frame completo usando raycasting
 * - mlx_put_image_to_window(): Función de MLX para mostrar la imagen
 */
int	motor(t_world *world)
{
	t_data	*data;

	data = world->data;
	char_movement(world);
	draw_image(world);
	mlx_put_image_to_window(data->mlx, data->window, data->img, 0, 0);
	return (1);
}

/**
 * draw_image - Renderiza una imagen completa del mundo 3D usando raycasting
 * 
 * @world: Puntero a la estructura principal con toda la información necesaria
 *         para el renderizado (mapa, posición/orientación del jugador, texturas).
 * 
 * Descripción:
 * Implementa el algoritmo completo de raycasting para generar una vista 3D
 * en perspectiva primera persona. Limpia primero el fondo con los colores de
 * cielo y suelo, luego lanza un rayo por cada columna de píxeles de la pantalla
 * (WINDOW_WIDTH rayos en total). Cada rayo determina qué pared es visible en
 * esa columna y a qué distancia, información usada para dibujar una línea
 * vertical texturizada de altura proporcional a la cercanía de la pared.
 * 
 * Retorno:
 * - void
 * 
 * Observaciones:
 * - El raycasting se hace de izquierda a derecha (columna 0 a WINDOW_WIDTH-1).
 * - La estructura t_column almacena temporalmente los datos de cada columna.
 * - Es la función más costosa computacionalmente del programa.
 * - El orden de renderizado (fondo primero, luego paredes) evita overdraw.
 * 
 * Funciones llamadas:
 * - clear_background(): Dibuja el cielo y suelo base
 * - one_ray(): Calcula la intersección del rayo con las paredes
 * - print_one_column(): Dibuja la columna vertical con la textura correcta
 */
void	draw_image(t_world *world)
{
	int			i;
	t_column	column;

	ft_bzero(&column, sizeof(t_column));
	clear_background(world);
	i = 0;
	while (i < WINDOW_WIDTH)
	{
		column.distance = one_ray(i, &(column.wall), world, &(column.impact));
		print_one_column(world, i, &column);
		i++;
	}
}

/**
 * clear_background - Rellena la imagen con los colores de cielo y suelo
 * 
 * @world: Puntero a la estructura que contiene los colores de cielo y suelo,
 *         así como el puntero a la imagen donde dibujar.
 * 
 * Descripción:
 * Dibuja el fondo base de la escena 3D antes de renderizar las paredes.
 * La mitad superior de la pantalla se rellena con el color del cielo
 * (ceiling_color) y la mitad inferior con el color del suelo (floor_color).
 * Esta división horizontal en el centro de la pantalla corresponde al horizonte
 * desde la perspectiva del jugador. Es más eficiente que calcular estos píxeles
 * durante el raycasting.
 * 
 * Retorno:
 * - void
 * 
 * Observaciones:
 * - Función static (solo visible dentro de motor.c).
 * - Los colores ya vienen parseados como enteros desde el archivo de configuración.
 * - Se ejecuta una vez por frame, antes del raycasting.
 * - Podría optimizarse con memset o dibujando rectángulos si MLX lo permitiera.
 * 
 * Funciones llamadas:
 * - ft_pixel_put(): Escribe un píxel individual en el buffer de imagen
 */
static void	clear_background(t_world *world)
{
	int		x;
	int		y;

	y = 0;
	while (y < WINDOW_HEIGHT / 2)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			ft_pixel_put(world->data, x, y, world->textures.ceiling_color);
			x++;
		}
		y++;
	}
	y = WINDOW_HEIGHT / 2;
	while (y < WINDOW_HEIGHT)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			ft_pixel_put(world->data, x, y, world->textures.floor_color);
			x++;
		}
		y++;
	}
}


/**
 * one_ray - Lanza un único rayo desde la cámara y calcula su impacto con las paredes
 * 
 * @i: Índice de la columna en la pantalla (0 a WINDOW_WIDTH-1). Determina el ángulo 
 *     del rayo dentro del campo de visión de la cámara.
 * @wall: Puntero donde se almacena el tipo de pared impactada (NORTH, SOUTH, EAST, 
 *        WEST, o ERROR si el rayo sale del mapa).
 * @world: Puntero a la estructura principal que contiene el estado del mundo (mapa,
 *         posición del jugador, dirección de vista, vector del plano de cámara).
 * @impact_on_wall: Puntero donde se almacena la coordenada exacta del impacto en la
 *                  pared (0.0 a textura.width-1), usado para el mapeo de texturas.
 * 
 * Descripción:
 * Implementa el algoritmo DDA (Digital Differential Analyzer) para raycasting. 
 * Calcula la dirección del rayo combinando el vector de dirección del jugador con
 * una porción del vector del plano de cámara (evitando el efecto ojo de pez).
 * El rayo avanza por la cuadrícula del mapa hasta encontrar una pared ('1') o
 * salir de los límites del mapa.
 * 
 * Retorno:
 * - Distancia perpendicular desde el jugador hasta la pared impactada.
 * - 7470000.747 si el rayo sale del mapa (valor arbitrario para indicar error).
 * 
 * Observaciones:
 * - La distancia retornada es perpendicular para evitar el efecto ojo de pez.
 * - El valor de plane_portion varía de -1.0 a 1.0 across the screen width.
 * - Esta función se llama WINDOW_WIDTH veces por frame (una por columna).
 * - El bucle while(1) está garantizado de terminar por los límites del mapa.
 * 
 * Funciones llamadas:
 * - init_ray(): Inicializa los parámetros del rayo para el algoritmo DDA
 * - hit_on_wall(): Calcula la distancia final y determina qué cara de la pared fue impactada
 * - go_further(): Avanza el rayo al siguiente cruce de líneas de la cuadrícula
 */
double	one_ray(int i, t_wall *wall, t_world *world, double *impact_on_wall)
{
	t_ray		ray;
	double		plane_portion;
	const char	**map = (const char **)world->map;

	//Calcular direccion en que saldra este rayo. Si los rayos fueran lanzados en angulos uniformes
	// tendrioamos un ojo de pez. Los rayos deben lanzarse de forma que crucen puntos equidistantes
	// a lo largo del plano de camara, como lo hacemos?
	//		Plane_portion, contiene la magnitud del vector del plano de camara, erntre -1 y +1
	//		Cada i tiene una magnitud, para i0 (el rayo mas a la izquierda de todos, la magnitud será -1)
	plane_portion = i * 2.0 / WINDOW_WIDTH - 1.0;
	//Construimos la dirección del rayo combinando dirección principal + desviación lateral:
	ray.ray_dir.x = world->char_direction.x + world->plane_direction.x * plane_portion;
	ray.ray_dir.y = world->char_direction.y	+ world->plane_direction.y * plane_portion;
	//Esto no solo ha modificado la direccion del rayo. Tambien a aumentado su magnitud
	// despues en init_ray() ray->delta_dist_x = fabs(1 / ray_dir.x); 
	// cuanto mayor sea ray_dir.x, menor sera delta_dist. De manera que cada paso
	// aumenta un poco menos la distancia. El resultado, redondisimo
	// consiste en que el valor almacenado en side_dist_x es la distancia perpendicular
	// entre el plano de camara y el lugar donde impacto el rallo 
	init_ray(&(world->char_position), &(ray.ray_dir), &(ray));
	while (1)
	{
		if (ray.tile_x < 0 || ray.tile_y < 0 || ray.tile_x >= (world->map_width)
			|| ray.tile_y >= (world->map_height))
		{
			*wall = ERROR;
			return (7470000.747);
		}
		if (map[ray.tile_y][ray.tile_x] == '1')
		{
			return (hit_on_wall(&ray, world, wall, impact_on_wall));
		}
		go_further(&ray, wall);
	}
}
