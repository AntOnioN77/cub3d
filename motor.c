/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:09:58 by antofern          #+#    #+#             */
/*   Updated: 2025/07/17 17:13:34 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

//LLamada iterativamente por mlx_loop_hook() debe actualizar la imagen y
// empujarla a la ventana 
int	motor(t_world *world)
{
	t_data	*data;

	data = world->data;
	char_movement(world);
	//Redibujar imagen
	draw_image(world);
	//Poner la imagen en la ventana
	mlx_put_image_to_window(data->mlx, data->window, data->img, 0, 0);
	return (1);
}

void	draw_image(t_world *world)
{
	int		i;
	double	distance;
	double	impact_on_wall;
	t_wall	wall;

	i = 0;
	while (i < WINDOW_WIDTH)
	{
		distance = one_ray(i, &wall, world, &impact_on_wall);
		print_one_column(world, i, distance, wall, impact_on_wall);
		i++;
	}
}

double	calculate_impact_on_wall(t_vector *ray_dir, t_wall wall,
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
	return ((impact_on_wall - (int)impact_on_wall) * (TEXTURE_WIDTH - 1));
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

double	one_ray(int i, t_wall *wall, t_world *world, double *impact_on_wall)
{
	t_ray		ray;
	double		plane_portion;
	const char	**map = (const char **)world->map;

	plane_portion = i * 2.0 / WINDOW_WIDTH - 1.0;
	ray.ray_dir.x = world->char_direction.x
		+ world->plane_direction.x * plane_portion;
	ray.ray_dir.y = world->char_direction.y
		+ world->plane_direction.y * plane_portion;
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
			return (hit_on_wall(&ray, world, wall, impact_on_wall));
		go_further(&ray, wall);
	}
}

double	hit_on_wall(t_ray *ray, t_world *world, t_wall *wall,
	double *impact_on_wall)
{
	if (*wall == VERTICAL)
	{
		set_wall_type(wall, &ray->ray_dir);
		*impact_on_wall = calculate_impact_on_wall(&ray->ray_dir, *wall, world,
				ray->side_dist_x - ray->delta_dist_x);
		return (ray->side_dist_x - ray->delta_dist_x);
	}
	if (*wall == HORIZONTAL)
	{
		set_wall_type(wall, &ray->ray_dir);
		*impact_on_wall = calculate_impact_on_wall(&ray->ray_dir, *wall,
				world, ray->side_dist_y - ray->delta_dist_y);
		return (ray->side_dist_y - ray->delta_dist_y);
	}
}

void	 init_ray(t_vector *char_position, const t_vector *vector, t_ray *ray)
{
	//En que baldosa del mapa comienza el rayo (su valor se ira incrementando al lo largo del DDA):
	ray->tile_x = (int)char_position->x;
	ray->tile_y = (int)char_position->y;
	//A lo largo del rayo, cual es la distancia entre dos lineas del mismo eje:
	if (vector->x == 0.0)
		ray->delta_dist_x = MAX_RAY_DISTANCE;
	else
		ray->delta_dist_x = fabs(1 / vector->x);
	if (vector->y == 0.0)
		ray->delta_dist_y = MAX_RAY_DISTANCE;
	else
		ray->delta_dist_y = fabs(1 / vector->y);
	//Cuanto falta hasta la proxima linea ray->side_dist_ :
	calc_side_dist(ray, char_position, vector);
}

// ray->side_dist_x:Cuanto falta hasta la proxima linea
// ray->step_x:nos mobemos a delante(1) o acia detras(-1) en x
void	calc_side_dist(t_ray *ray, t_vector *char_position,
	const t_vector *vector)
{
	if (vector->x > 0)
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->tile_x + 1.0 - char_position->x )
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
		ray->side_dist_y = (ray->tile_y  + 1.0 - char_position->y)
			* ray->delta_dist_y;
	}
	else
	{
		ray->step_y = -1;
		ray->side_dist_y = (char_position->y - ray->tile_y)
			* ray->delta_dist_y;
	}
}

void	set_wall_type(t_wall *wall, t_vector *vector)
{
	if (*wall == VERTICAL)
	{
		if (vector->x > 0)
			*wall = EAST;
		else
			*wall = WEST;
	}
	else if (*wall == HORIZONTAL)
	{
		if (vector->y > 0)
			*wall = NORTH;
		else
			*wall = SOUTH;
	}
}
