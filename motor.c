/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:09:58 by antofern          #+#    #+#             */
/*   Updated: 2025/07/16 16:57:42 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

//LLamada iterativamente por mlx_loop_hook() debe actualizar la imagen y
// empujarla a la ventana 
int	motor(t_world *world)
{
	t_data *data;
	data = world->data;

	//Verifica cual es la ultima tecla que se pulso (y permanece pulsada)
	// -Actualiza el valor en world.char_direction
	// -Actualiza el valor en world.char_position
	char_movement(world);

	//Redibujar imagen
	draw_image(world);
	//Poner la imagen en la ventana
	mlx_put_image_to_window(data->mlx, data->window, data->img, 0, 0);
	return (1);

}

void draw_image(t_world *world)
{
	int i;
	double distance;
	double impact_on_wall;
	t_wall wall;

	i = 0;
	while(i < WINDOW_WIDTH)
	{
		distance = one_ray(i, &(wall), world, &impact_on_wall);

		print_one_column(world, i, distance, wall, impact_on_wall);
		i++;
	}
}

double calculate_impact_on_wall(t_vector *ray_dir, t_wall wall, t_world *world, double distance)
{
	double impact_on_wall = 0.0;	
	if(wall == NORTH || wall == SOUTH)
		impact_on_wall = world->char_position.x + (distance * ray_dir->x);
	else if(wall == WEST || wall == EAST)
		impact_on_wall = world->char_position.y + (distance * ray_dir->y);
	else
		perror("Error: one_ray maybe shot to infinite.\n");
	return( (impact_on_wall - (int)impact_on_wall) * (TEXTURE_WIDTH - 1)); //de 0 a 63

}

double	one_ray(int i, t_wall *wall, t_world *world, double *impact_on_wall) //demasiados parametros, a ver que struct monto, para que quede limpio y eficiente
{
	t_ray ray;
	t_vector ray_dir;
	double plane_portion;
	const char **map = (const char **)world->map;
	t_vector *char_position = &(world->char_position);

	plane_portion = i * 2.0 / WINDOW_WIDTH - 1.0; //de -1 a 1
	ray_dir.x = world->char_direction.x + world->plane_direction.x * plane_portion;
	ray_dir.y = world->char_direction.y + world->plane_direction.y * plane_portion;
	init_ray(char_position, &ray_dir, &(ray));
if(DEBUGMODE){printf("side_dist_x:%f, side_dist_y:%f\n", ray.side_dist_x, ray.side_dist_y);}
if(DEBUGMODE){printf("delta_dist_x:%f, delta_dist_y:%f\n", ray.delta_dist_x, ray.delta_dist_y);}
	while(1)
	{
		if(ray.tile_x < 0 || ray.tile_y < 0 || ray.tile_x >= (world->map_width) || ray.tile_y >= (world->map_height))
		{
if(DEBUGMODE){printf("map[%d][%d] ha explotado.\n", ray.tile_y, ray.tile_x);}
			*wall = ERROR;
			return 7470000.747;//error
		}
		if(map[ray.tile_y][ray.tile_x] == '1')
		{
			if(*wall == VERTICAL)
			{
if(DEBUGMODE){printf("ray.tile_x - char_position->x + (1 - ray.step_x) / 2) / vector->x \n %d - %f + (1 - %d) / 2) / %f\n", ray.tile_x, char_position->x, ray.step_x, ray_dir.x);}
if(DEBUGMODE){printf("     --VERTICAL--\n ray.step_y:%d, ray.tile_y:%d, ray.side_dist_y:%f\n", ray.step_y, ray.tile_y, ray.side_dist_y);}
				set_wall_type(wall, &ray_dir);
				*impact_on_wall = calculate_impact_on_wall(&ray_dir, *wall, world, ray.side_dist_x - ray.delta_dist_x);// NO HACER LA RESTA DOS VECES. prueba
				return(ray.side_dist_x - ray.delta_dist_x);
				//return((ray.tile_x - char_position->x + (1 - ray.step_x) / 2) / char_dir.x);
			}
			if(*wall == HORIZONTAL)
			{
if(DEBUGMODE){printf("ray.tile_y - char_position->y + (1 - ray.step_y) / 2) / vector->y \n %d - %f + (1 - %d) / 2) / %f\n", ray.tile_y, char_position->y, ray.step_y, ray_dir.y);}
if(DEBUGMODE){printf("     --HORIZONTAL--\n ray.step_y:%d, ray.tile_y:%d, ray.side_dist_y:%f\n", ray.step_y, ray.tile_y, ray.side_dist_y);}
				set_wall_type(wall, &ray_dir);
				*impact_on_wall = calculate_impact_on_wall(&ray_dir, *wall, world, ray.side_dist_y - ray.delta_dist_y);
				return(ray.side_dist_y - ray.delta_dist_y);//así lo resulve lode, VERIFICA QUE FUNCIONA!!, deberia absorver el ojo de pez
				//return((ray.tile_y - char_position->y + (1 - ray.step_y) / 2) / char_dir.y);
			}
		}
		if(ray.side_dist_x < ray.side_dist_y)
		{
			ray.side_dist_x += ray.delta_dist_x;
			ray.tile_x += ray.step_x;
			*wall = VERTICAL;
		}
		else
		{
			ray.side_dist_y += ray.delta_dist_y;
			ray.tile_y += ray.step_y;
			*wall = HORIZONTAL;
		}
	}
}


void init_ray(t_vector *char_position, const t_vector *vector, t_ray *ray)
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
void calc_side_dist(t_ray *ray, t_vector *char_position, const t_vector *vector)
{
	if (vector->x > 0)
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->tile_x + 1.0 - char_position->x ) * ray->delta_dist_x; 
	}
	else
	{
		ray->step_x = -1;
		ray->side_dist_x = (char_position->x - ray->tile_x) * ray->delta_dist_x; 
	}
	if (vector->y > 0)
	{
		ray->step_y = 1;
		ray->side_dist_y = ( ray->tile_y  + 1.0 - char_position->y) * ray->delta_dist_y; 
	}
	else
	{
		ray->step_y = -1;
		ray->side_dist_y = (char_position->y - ray->tile_y) * ray->delta_dist_y; 
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
