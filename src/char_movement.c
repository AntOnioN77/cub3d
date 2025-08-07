/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_movement.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:50:51 by antofern          #+#    #+#             */
/*   Updated: 2025/08/07 14:16:55 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

static int	is_wall(double x, double y, char **map)
{
	return (map[(int)y][(int)x] == '1');
}

static int	available_position(double x, double y, t_world *world)
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

void	set_position(double new_x, double new_y, t_world *world)
{
	if (!available_position(new_x, new_y, world))
	{
		world->char_position.x = new_x;
		world->char_position.y = new_y;
	}
	else if (!available_position(new_x, world->char_position.y, world))
		world->char_position.x = new_x;
	else if (!available_position(world->char_position.x, new_y, world))
		world->char_position.y = new_y;
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
