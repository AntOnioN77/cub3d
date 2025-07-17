/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_movement.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:50:51 by antofern          #+#    #+#             */
/*   Updated: 2025/07/17 16:57:50 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	rotate_normalize_vector(t_vector *vector, double sin, double cos);

void	char_movement(t_world *world)
{
	if (world->key == 'd')
	{
		rotate_normalize_vector(&(world->char_direction), sin(ROT_STEP),
			cos(ROT_STEP));
		rotate_vector(&(world->plane_direction), sin(ROT_STEP), cos(ROT_STEP));
	}
	else if (world->key == 'a')
	{
	}
	else if (world->key == 'w')
	{
		// Avanzar en la direccion del personaje
		world->char_position.x += world->char_direction.x * MOVE_STEP;
		world->char_position.y += world->char_direction.y * MOVE_STEP;
	}
	else if (world->key == 's')
	{
		// Retroceder en la direccion opuesta del personaje
		world->char_position.x -= world->char_direction.x * MOVE_STEP;
		world->char_position.y -= world->char_direction.y * MOVE_STEP;
	}
}

void	rotate_vector(t_vector *vector, double sin, double cos)
{
	double	result_x;
	double	result_y;

	result_x = vector->x * cos - vector->y * sin;
	result_y = vector->x * sin + vector->y * cos;
	vector->x = result_x;
	vector->y = result_y;
}

/*esta es la formula para rotar en sentido horario un vector
x' = x * cos(θ) - y * sin(θ)
y' = x * sin(θ) + y * cos(θ)*/
void	rotate_normalize_vector(t_vector *vector, double sin, double cos)
{
	double	result_x;
	double	result_y;
	double	magnitude;

	result_x = vector->x * cos - vector->y * sin;
	result_y = vector->x * sin + vector->y * cos;
	magnitude = sqrt(result_x * result_x + result_y * result_y);
	vector->x = result_x / magnitude;
	vector->y = result_y / magnitude;
}

