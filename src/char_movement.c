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

#include "../inc/cub.h"

void    char_movement(t_world *world)
{
	/* Giro a la derecha */
	if (world->key_down['d'])
	{
		rotate_vector(&world->char_direction, ROT_SIN, ROT_COS);
		rotate_vector(&world->plane_direction, ROT_SIN, ROT_COS);
	}
	/* Giro a la izquierda */
	if (world->key_down['a'])
	{
		/* rota con -ROT_STEP */
		rotate_vector(&world->char_direction, -ROT_SIN, ROT_COS);
		rotate_vector(&world->plane_direction, -ROT_SIN, ROT_COS);
	}
	/* Avanzar */
	if (world->key_down['w'])
	{
		world->char_position.x += world->char_direction.x * MOVE_STEP;
		world->char_position.y += world->char_direction.y * MOVE_STEP;
	}
	/* Retroceder */
	if (world->key_down['s'])
	{
		world->char_position.x -= world->char_direction.x * MOVE_STEP;
		world->char_position.y -= world->char_direction.y * MOVE_STEP;
	}
}

void	rotate_vector(t_vector *v, double sinv, double cosv)
{
	double	nx;
	double	ny;

	nx = v->x * cosv - v->y * sinv;
	ny = v->x * sinv + v->y * cosv;
	v->x = nx;
	v->y = ny;
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

