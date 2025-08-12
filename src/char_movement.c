/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_movement.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:50:51 by antofern          #+#    #+#             */
/*   Updated: 2025/08/09 23:41:08 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

static int	aviable_position(double x, double y, t_world *world);
static int	is_wall(double x, double y, char **map);

/**
 * char_movement - Procesa el input del teclado y actualiza la posición/rotación del jugador
 * 
 * @world: Puntero a la estructura principal que contiene el estado del jugador
 *         (posición, dirección, plano de cámara) y el array de teclas presionadas.
 * 
 * Descripción:
 * Lee el estado del array key_down[] para determinar qué teclas están presionadas
 * y ejecuta las acciones correspondientes. Las teclas 'a' y 'd' rotan la cámara
 * del jugador (y su plano de cámara) mientras que 'w' y 's' mueven al jugador
 * hacia adelante y atrás en la dirección que está mirando. Los movimientos se
 * calculan pero no se aplican directamente; se validan a través de set_position()
 * para evitar atravesar paredes. Múltiples teclas pueden estar presionadas
 * simultáneamente.
 * 
 * Retorno:
 * - void
 * 
 * Observaciones:
 * - Se ejecuta una vez por frame antes del renderizado.
 * - ROT_SIN y ROT_COS definen la velocidad de rotación (radianes por frame).
 * - STEP define la velocidad de movimiento (unidades del mapa por frame).
 * - La rotación afecta tanto char_direction como plane_direction para mantener perpendiculares.
 * - El movimiento es relativo a la dirección actual del jugador, no a los ejes del mapa.
 * 
 * Funciones llamadas:
 * - rotate_vector(): Aplica rotación 2D a los vectores de dirección
 * - set_position(): Valida y aplica el nuevo movimiento evitando colisiones
 */
void	char_movement(t_world *world)
{
	t_vector	new_pos;

	new_pos = world->char_position;
	if (world->key_down['d'])
	{
		rotate_vector(&world->char_direction, ROT_SIN, ROT_COS);
		rotate_vector(&world->plane_direction, ROT_SIN, ROT_COS);
	}
	if (world->key_down['a'])
	{
		rotate_vector(&world->char_direction, -ROT_SIN, ROT_COS);
		rotate_vector(&world->plane_direction, -ROT_SIN, ROT_COS);
	}
	if (world->key_down['w'])
	{
		new_pos.x = world->char_position.x + world->char_direction.x * STEP;
		new_pos.y = world->char_position.y + world->char_direction.y * STEP;
	}
	if (world->key_down['s'])
	{
		new_pos.x = world->char_position.x - world->char_direction.x * STEP;
		new_pos.y = world->char_position.y - world->char_direction.y * STEP;
	}
	set_position(new_pos.x, new_pos.y, world);
}

/**
 * set_position - Valida y establece la nueva posición del jugador evitando colisiones
 * 
 * @new_x: Coordenada X deseada para el jugador en el mapa.
 * @new_y: Coordenada Y deseada para el jugador en el mapa.
 * @world: Puntero a la estructura con la posición actual del jugador y el mapa.
 * 
 * Descripción:
 * Intenta mover al jugador a la posición (new_x, new_y) pero verifica primero
 * que no haya colisión con paredes. Si el movimiento completo es válido, lo aplica.
 * Si no, intenta un "wall sliding": permite el movimiento en un solo eje si es
 * posible. Esto permite al jugador deslizarse junto a las paredes diagonalmente
 * cuando se mueve en ángulo, mejorando la jugabilidad. Si ningún movimiento es
 * posible, la posición no cambia.
 * 
 * Retorno:
 * - void
 * 
 * Observaciones:
 * - El wall sliding mejora significativamente la experiencia de juego.
 * - Primero intenta mover en ambos ejes, luego solo X, luego solo Y.
 * - Si todos fallan, el jugador queda en su posición original.
 * - aviable_position() verifica un área circular alrededor del punto.
 * 
 * Funciones llamadas:
 * - aviable_position(): Verifica si una posición está libre de colisiones
 */
void	set_position(double new_x, double new_y, t_world *world)
{
	if (!aviable_position(new_x, new_y, world))
	{
		world->char_position.x = new_x;
		world->char_position.y = new_y;
	}
	else if (!aviable_position(new_x, world->char_position.y, world))
		world->char_position.x = new_x;
	else if (!aviable_position(world->char_position.x, new_y, world))
		world->char_position.y = new_y;
}

/**
 * aviable_position - Verifica si una posición está libre de colisiones con paredes
 * 
 * @x: Coordenada X central a verificar en el mapa.
 * @y: Coordenada Y central a verificar en el mapa.
 * @world: Puntero a la estructura que contiene el mapa.
 * 
 * Descripción:
 * Verifica que el jugador pueda ocupar la posición (x,y) sin colisionar con
 * paredes. No revisa solo el punto central, sino un área circular de radio
 * PLAYER_RADIUS alrededor del punto, verificando 8 puntos en las direcciones
 * cardinales e intercardinales. Esto evita que el jugador se acerque demasiado
 * a las paredes y previene glitches visuales o que partes del jugador atraviesen
 * geometría.
 * 
 * Retorno:
 * - 0 si la posición es válida (no hay colisión).
 * - 1 si hay colisión con alguna pared.
 * 
 * Observaciones:
 * - Función static (solo visible dentro de char_movement.c).
 * - PLAYER_RADIUS define el tamaño de colisión del jugador.
 * - Verifica 8 puntos formando un octágono alrededor del centro.
 * - El OR lógico hace que baste una colisión para rechazar la posición.
 * - Previene el bug común de quedarse atascado en esquinas.
 * 
 * Funciones llamadas:
 * - is_wall(): Comprueba si un punto específico es pared
 */
static int	aviable_position(double x, double y, t_world *world)
{
	if (is_wall(x + PLAYER_RADIUS, y, world->map)
		|| is_wall(x + PLAYER_RADIUS, y + PLAYER_RADIUS, world->map)
		|| is_wall(x, y + PLAYER_RADIUS, world->map)
		|| is_wall(x - PLAYER_RADIUS, y + PLAYER_RADIUS, world->map)
		|| is_wall(x - PLAYER_RADIUS, y, world->map)
		|| is_wall(x - PLAYER_RADIUS, y - PLAYER_RADIUS, world->map)
		|| is_wall(x, y - PLAYER_RADIUS, world->map)
		|| is_wall(x + PLAYER_RADIUS, y - PLAYER_RADIUS, world->map))
		return (1);
	return (0);
}

/**
 * is_wall - Comprueba si una coordenada del mapa contiene una pared
 * 
 * @x: Coordenada X en el mapa (puede tener decimales).
 * @y: Coordenada Y en el mapa (puede tener decimales).
 * @map: Array bidimensional de caracteres representando el mapa.
 * 
 * Descripción:
 * Convierte las coordenadas flotantes a enteros (truncando) para obtener
 * los índices del tile en el mapa, y verifica si ese tile es una pared
 * (carácter '1'). Es la verificación más básica de colisión, usada como
 * building block para detección de colisiones más complejas.
 * 
 * Retorno:
 * - 1 si la posición corresponde a una pared ('1').
 * - 0 si la posición es transitable (no es '1').
 * 
 * Observaciones:
 * - Función static (solo visible dentro de char_movement.c).
 * - El cast a int trunca, no redondea (2.9 se convierte en 2).
 * - Asume que el mapa solo contiene '0' (vacío) y '1' (pared) en este punto.
 * - No verifica límites del mapa (asume que las coordenadas son válidas).
 * 
 * Funciones llamadas:
 * - Ninguna
 */
static int	is_wall(double x, double y, char **map)
{
	return (map[(int)y][(int)x] == '1');
}

/**
 * rotate_vector - Aplica una rotación 2D a un vector
 * 
 * @v: Puntero al vector a rotar (se modifica in-place).
 * @sinv: Seno del ángulo de rotación.
 * @cosv: Coseno del ángulo de rotación.
 * 
 * Descripción:
 * Aplica una matriz de rotación 2D estándar al vector v. La rotación se
 * realiza en sentido antihorario si sinv es positivo. La fórmula utilizada es:
 * x' = x*cos(θ) - y*sin(θ)
 * y' = x*sin(θ) + y*cos(θ)
 * Se usa una variable temporal para evitar usar el valor modificado de x
 * en el cálculo de y.
 * 
 * Retorno:
 * - void
 * 
 * Observaciones:
 * - Los valores sin/cos se precalculan como constantes (ROT_SIN, ROT_COS) para eficiencia.
 * - Se usa para rotar tanto el vector dirección como el plano de cámara.
 * - La rotación es incremental (se aplica frame a frame mientras la tecla está presionada).
 * - Mantiene la magnitud del vector (rotación pura sin escalado).
 * 
 * Funciones llamadas:
 * - Ninguna
 */
void	rotate_vector(t_vector *v, double sinv, double cosv)
{
	double	nx;
	double	ny;

	nx = v->x * cosv - v->y * sinv;
	ny = v->x * sinv + v->y * cosv;
	v->x = nx;
	v->y = ny;
}
