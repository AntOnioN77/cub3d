/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_movement.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:50:51 by antofern          #+#    #+#             */
/*   Updated: 2025/07/29 23:53:47 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

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

void	set_position(double new_position_x, double new_position_y,
	t_world *world)
{
	double	next_step_x;
	double	next_step_y;

	if ((int)new_position_y < (world->map_height)
		&& (int)new_position_x < (world->map_width)
		&& new_position_y >= 0 && new_position_x >= 0)
	{
		if (new_position_x - (int)new_position_x > 0.5)
			next_step_x = new_position_x + 0.04;
		else
			next_step_x = new_position_x - 0.04;
		if (new_position_y - (int)new_position_y > 0.5)
			next_step_y = new_position_y + 0.04;
		else
			next_step_y = new_position_y - 0.04;
		if (world->map[(int)(new_position_y)][(int)(new_position_x)] == '0' //por si se rompe la cabeza contra una esquina XDDD
			&& world->map[(int)(next_step_y)][(int)(next_step_x)] == '0'
			&& world->map[(int)(next_step_y)][(int)(new_position_x)] == '0'
			&& world->map[(int)(new_position_y)][(int)(next_step_x)] == '0')
		{
			world->char_position.x = new_position_x;
			world->char_position.y = new_position_y;
		}
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
