/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:09:58 by antofern          #+#    #+#             */
/*   Updated: 2025/07/14 12:36:58 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"






//Funcion que toma las distancias de cada rayo y dibuja una columna roja de un tamaño proporcional
// a la distancia del rayo, en la imagen de la ventana, el cielo y el suelo son tonos de gris
// y el color de la pared es rojo
void print_columns(double distances[], t_data *data, t_textures textures)
{
	//por hacer
}


//LLamada iterativamente por mlx_loop_hook() debe actualizar la imagen y
// empujarla a la ventana 
int	motor(t_world *world)
{
	t_data *data;
	t_vector screen_vectors[WINDOW_WIDTH + 1]; //+1 para evitar overflow

	data = world->data;

	//Verifica cual es la ultima tecla que se pulso (y permanece pulsada)
	//si fue un giro de camara <- o -> 
	// -Actualiza el valor en world.char_direction
	// -Recalcula screen_vectors
	check_loock(world);

	recalculate_vectors(&(world->char_direction),&(world->plane_direction), screen_vectors);

	//Redibujar imagen
	draw_image(world);
if(DEBUGMODE){sleep(2);}

	mlx_put_image_to_window(data->mlx, data->window, data->img, 0, 0);
	return (1);

}

void draw_image(t_world *world)
{
	int i;
	double distances[WINDOW_WIDTH];
	t_wall wall[WINDOW_WIDTH];

	i = 0;
//	calculate_distances(world, screen_vectors, distances, wall);
	while(i < WINDOW_WIDTH)
	{
if(DEBUGMODE){printf("-------------ray[%d]-----------\n", i);}
		distances[i] = one_ray((const char**)world->map, &(world->char_position), i, &(wall[i]), world);
if(DEBUGMODE){printf("	distance[%d] == %f\n",i, distances[i]);}
		i++;
	}
	
	print_columns(distances, world->data, world->textures);
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


double	one_ray(const char **map, t_vector *char_position, int i, t_wall *wall, t_world *world)
{
	t_ray ray;
	t_vector vector;
	double plane_portion;

	plane_portion = i * 2.0 / WINDOW_WIDTH - 1.0; //de -1 a 1
	vector.x = world->char_direction.x + world->plane_direction.x * plane_portion;
	vector.y = world->char_direction.y + world->plane_direction.y * plane_portion;
	init_ray(char_position, &vector, &(ray));
if(DEBUGMODE){printf("side_dist_x:%f, side_dist_y:%f\n", ray.side_dist_x, ray.side_dist_y);}
if(DEBUGMODE){printf("delta_dist_x:%f, delta_dist_y:%f\n", ray.delta_dist_x, ray.delta_dist_y);}
	while(1)
	{
		if(ray.tile_x < 0 || ray.tile_y < 0 || ray.tile_x >= (world->map_width) || ray.tile_y >= (world->map_height))
		{
if(DEBUGMODE){printf("map[%d][%d] ha explotado.\n", ray.tile_y, ray.tile_x);}
			*wall = 7;
			return 7470000.747;
		}
		if(map[ray.tile_y][ray.tile_x] == '1')
		{
			if(*wall == VERTICAL)
			{
if(DEBUGMODE){printf("ray.tile_x - char_position->x + (1 - ray.step_x) / 2) / vector->x \n %d - %f + (1 - %d) / 2) / %f\n", ray.tile_x, char_position->x, ray.step_x, vector.x);}
if(DEBUGMODE){printf("     --VERTICAL--\n ray.step_y:%d, ray.tile_y:%d, ray.side_dist_y:%f\n", ray.step_y, ray.tile_y, ray.side_dist_y);}
				return((ray.tile_x - char_position->x + (1 - ray.step_x) / 2) / vector.x);
			}
			if(*wall == HORIZONTAL)
			{
if(DEBUGMODE){printf("ray.tile_y - char_position->y + (1 - ray.step_y) / 2) / vector->y \n %d - %f + (1 - %d) / 2) / %f\n", ray.tile_y, char_position->y, ray.step_y, vector.y);}
if(DEBUGMODE){printf("     --HORIZONTAL--\n ray.step_y:%d, ray.tile_y:%d, ray.side_dist_y:%f\n", ray.step_y, ray.tile_y, ray.side_dist_y);}
				return((ray.tile_y - char_position->y + (1 - ray.step_y) / 2) / vector.y);
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

/*//innecesaria
void calculate_distances(t_world *world, const t_vector vectors[], double distances[], t_wall *wall)
{
	int i;

	i = 0;
	while(i < WINDOW_WIDTH)
	{
printf("-------------distance[%d]-----------\n",i);
		distances[i] = one_ray((const char**)world->map, &(world->char_position) , &(vectors[i]), &(wall[i]), world);
printf("	distance[%d] == %f\n",i, distances[i]);
		i++;
	}
}
*/
