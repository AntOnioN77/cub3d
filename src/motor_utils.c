/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motor_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:00:25 by antofern          #+#    #+#             */
/*   Updated: 2025/08/07 15:08:54 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

void	init_ray(t_vector *char_position, const t_vector *vector, t_ray *ray)
{
	ray->tile_x = (int)char_position->x;
	ray->tile_y = (int)char_position->y;
	if (vector->x == 0.0)
		ray->delta_dist_x = MAX_RAY_DISTANCE;
	else
		ray->delta_dist_x = fabs(1 / vector->x);
	if (vector->y == 0.0)
		ray->delta_dist_y = MAX_RAY_DISTANCE;
	else
		ray->delta_dist_y = fabs(1 / vector->y);
	calc_side_dist(ray, char_position, vector);
}

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

// ray->side_dist_x: How far to the next gridline in x
// ray->step_x: We move forward (1) or backward (-1) in x
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
