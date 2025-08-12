/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motor_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:00:25 by antofern          #+#    #+#             */
/*   Updated: 2025/08/10 18:55:58 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

/**
 * init_ray - Inicializa los parámetros del rayo para el algoritmo DDA
 * 
 * @char_position: Puntero al vector con la posición actual del jugador en el mapa
 *                 (coordenadas x,y en unidades del mapa).
 * @vector: Puntero al vector dirección del rayo normalizado (ray_dir calculado en one_ray).
 * @ray: Puntero a la estructura que almacenará todos los parámetros del rayo para DDA.
 * 
 * Descripción:
 * Prepara todos los valores necesarios para ejecutar el algoritmo DDA (Digital 
 * Differential Analyzer). Calcula las distancias entre cruces de líneas de la 
 * cuadrícula (delta_dist), determina la casilla inicial del mapa donde está el 
 * jugador (tile_x, tile_y), y calcula las distancias hasta los primeros cruces
 * de líneas verticales y horizontales (side_dist). Maneja casos especiales donde
 * el rayo es perfectamente horizontal o vertical asignando MAX_RAY_DISTANCE.
 * 
 * Retorno:
 * - void
 * 
 * Observaciones:
 * - delta_dist representa cuánto debe avanzar el rayo para cruzar una línea de cuadrícula.
 * - Los valores infinitos (división por 0) se manejan con MAX_RAY_DISTANCE.
 * - fabs() se usa porque delta_dist siempre debe ser positivo (es una distancia).
 * - Esta función es crítica para el rendimiento al ser llamada para cada rayo.
 * 
 * Funciones llamadas:
 * - calc_side_dist(): Calcula las distancias iniciales y direcciones de paso
 */
void	init_ray(t_vector *char_position, const t_vector *vector, t_ray *ray)
{
	ray->tile_x = (int)char_position->x;
	ray->tile_y = (int)char_position->y;
	if (vector->x == 0.0)
		ray->delta_dist_x = MAX_RAY_DISTANCE;
	else
		ray->delta_dist_x = fabs(1 / vector->x); //
	if (vector->y == 0.0)
		ray->delta_dist_y = MAX_RAY_DISTANCE;
	else
		ray->delta_dist_y = fabs(1 / vector->y);
	calc_side_dist(ray, char_position, vector);
}

/**
 * hit_on_wall - Procesa el impacto del rayo con una pared y calcula los datos finales
 * 
 * @ray: Puntero a la estructura con el estado actual del rayo tras impactar.
 * @world: Puntero a la estructura del mundo para acceder a la posición del jugador.
 * @wall: Puntero al tipo de pared (VERTICAL/HORIZONTAL en entrada, NORTH/SOUTH/EAST/WEST en salida).
 * @impact_on: Puntero donde se almacena la coordenada del impacto en la textura.
 * 
 * Descripción:
 * Cuando el algoritmo DDA detecta una colisión con una pared, esta función determina
 * exactamente qué cara de la pared fue impactada (norte, sur, este u oeste) basándose
 * en la dirección del rayo y si el impacto fue en una línea vertical u horizontal
 * de la cuadrícula. Calcula la distancia perpendicular real hasta la pared (restando
 * delta_dist para corregir el último paso del DDA) y determina el punto exacto del
 * impacto en la pared para el mapeo de texturas.
 * 
 * Retorno:
 * - Distancia perpendicular desde el jugador hasta el punto de impacto.
 * 
 * Observaciones:
 * - La función modifica el valor de *wall de VERTICAL/HORIZONTAL a NORTH/SOUTH/EAST/WEST.
 * - La resta de delta_dist corrige porque DDA da un paso más allá de la pared.
 * - Si wall no es VERTICAL ni HORIZONTAL, el programa termina (caso imposible).
 * - El cálculo de distancia perpendicular evita el efecto ojo de pez.
 * 
 * Funciones llamadas:
 * - calc_impact_on_wall(): Calcula la coordenada exacta del impacto para texturas
 */
double	hit_on_wall(t_ray *ray, t_world *world, t_wall *wall, double *impact_on)
{
	double	distance;

	if (*wall == VERTICAL)
	{
		distance = ray->side_dist_x - ray->delta_dist_x;
		if (ray->ray_dir.x > 0)
			*wall = EAST;
		else
			*wall = WEST;
		*impact_on = calc_impact_on_wall(&ray->ray_dir, *wall, world, distance);
		return (distance);
	}
	else if (*wall == HORIZONTAL)
	{
		distance = ray->side_dist_y - ray->delta_dist_y;
		if (ray->ray_dir.y > 0)
			*wall = NORTH;
		else
			*wall = SOUTH;
		*impact_on = calc_impact_on_wall(&ray->ray_dir, *wall, world, distance);
		return (distance);
	}
	else
		exit(1);
}

/**
 * calc_impact_on_wall - Calcula la coordenada exacta del impacto en la pared para texturas
 * 
 * @ray_dir: Puntero al vector dirección del rayo.
 * @wall: Tipo de pared impactada (NORTH, SOUTH, EAST, WEST).
 * @world: Puntero a la estructura del mundo para acceder a la posición del jugador.
 * @distance: Distancia perpendicular desde el jugador hasta la pared.
 * 
 * Descripción:
 * Determina en qué punto exacto de la pared impactó el rayo, necesario para saber
 * qué columna de píxeles de la textura debe dibujarse. Para paredes norte/sur usa
 * la coordenada X del impacto, para este/oeste usa la Y. Calcula el punto de impacto
 * proyectando la posición del jugador más la distancia en la dirección del rayo.
 * Extrae solo la parte decimal (posición dentro del tile) y la escala al ancho de
 * la textura.
 * 
 * Retorno:
 * - Coordenada en la textura (0 a textura.width-1) donde impactó el rayo.
 * 
 * Observaciones:
 * - La parte entera se descarta con (impact - (int)impact) para obtener solo la fracción.
 * - Se multiplica por (width-1) en lugar de width para evitar desbordamiento de índices.
 * - Si wall tiene un valor inesperado, se reporta error pero el programa continúa.
 * - El resultado se usa directamente como coordenada X en la textura.
 * 
 * Funciones llamadas:
 * - perror(): Reporta error si el tipo de pared es inválido
 */
double	calc_impact_on_wall(t_vector *ray_dir, t_wall wall,
	t_world *world, double distance)
{
	double	impact_on_wall;

	impact_on_wall = 0.0;
	if (wall == NORTH || wall == SOUTH)
		impact_on_wall = world->char_position.x + (distance * ray_dir->x);
	else if (wall == WEST || wall == EAST)
		impact_on_wall = world->char_position.y + (distance * ray_dir->y);
	else
		perror("Error: one_ray maybe shot to infinite.\n");
	return ((impact_on_wall - (int)impact_on_wall)
		* (world->textures.width - 1));
}

/**
 * go_further - Avanza el rayo al siguiente cruce de líneas de la cuadrícula
 * 
 * @ray: Puntero a la estructura del rayo con las distancias y posición actual.
 * @wall: Puntero donde se marca si el próximo cruce será vertical u horizontal.
 * 
 * Descripción:
 * Implementa el paso principal del algoritmo DDA. Compara las distancias hasta
 * el próximo cruce vertical (side_dist_x) y horizontal (side_dist_y), eligiendo
 * el más cercano. Avanza el rayo hasta ese cruce actualizando la distancia
 * acumulada y moviendo a la siguiente casilla del mapa en la dirección apropiada
 * (step_x o step_y). También marca si el cruce fue con una línea vertical u
 * horizontal, información necesaria para determinar qué cara de la pared se ve.
 * 
 * Retorno:
 * - void
 * 
 * Observaciones:
 * - Esta función se llama repetidamente hasta encontrar una pared.
 * - step_x/step_y son +1 o -1 según la dirección del rayo.
 * - La elección del mínimo entre side_dist garantiza no saltarse casillas.
 * - *wall se usa temporalmente para recordar el tipo de cruce entre llamadas.
 * 
 * Funciones llamadas:
 * - Ninguna
 */
void	go_further(t_ray *ray, t_wall *wall)
{
	if (ray->side_dist_x < ray->side_dist_y)
	{
		ray->side_dist_x += ray->delta_dist_x;
		ray->tile_x += ray->step_x;
		*wall = VERTICAL;
	}
	else
	{
		ray->side_dist_y += ray->delta_dist_y;
		ray->tile_y += ray->step_y;
		*wall = HORIZONTAL;
	}
}

/**
 * calc_side_dist - Calcula las distancias iniciales hasta los cruces de cuadrícula y direcciones
 * 
 * @ray: Puntero a la estructura donde se almacenarán side_dist_x/y y step_x/y.
 * @char_position: Puntero a la posición actual del jugador en el mapa.
 * @vector: Puntero al vector dirección del rayo.
 * 
 * Descripción:
 * Inicializa los valores de side_dist (distancia hasta el próximo cruce de línea)
 * y step (dirección de avance: +1 o -1) para ambos ejes. Si el rayo va hacia 
 * positivo en un eje, side_dist es la distancia hasta la próxima línea entera
 * superior; si va hacia negativo, es la distancia hasta la línea entera inferior
 * actual. Estos valores son fundamentales para que DDA funcione correctamente,
 * determinando cuándo y en qué dirección el rayo cruza las líneas de la cuadrícula.
 * 
 * Retorno:
 * - void
 * 
 * Observaciones:
 * - Los cálculos son diferentes según el signo de vector->x y vector->y.
 * - tile + 1.0 - position da la distancia hasta la próxima línea entera.
 * - position - tile da la distancia hasta la línea entera anterior.
 * - Se multiplica por delta_dist para convertir distancia en el mapa a distancia del rayo.
 * - Comentarios en inglés en el código original explican el significado de cada campo.
 * 
 * Funciones llamadas:
 * - Ninguna
 */
void	calc_side_dist(t_ray *ray, t_vector *char_position,
	const t_vector *vector)
{
	if (vector->x > 0)
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->tile_x + 1.0 - char_position->x)
			* ray->delta_dist_x;
	}
	else
	{
		ray->step_x = -1;
		ray->side_dist_x = (char_position->x - ray->tile_x)
			* ray->delta_dist_x;
	}
	if (vector->y > 0)
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->tile_y + 1.0 - char_position->y)
			* ray->delta_dist_y;
	}
	else
	{
		ray->step_y = -1;
		ray->side_dist_y = (char_position->y - ray->tile_y)
			* ray->delta_dist_y;
	}
}
