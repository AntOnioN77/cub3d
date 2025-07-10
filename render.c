/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:09:58 by antofern          #+#    #+#             */
/*   Updated: 2025/07/10 20:38:28 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

//LLamada iterativamente por mlx_loop_hook() debe actualizar la imagen y
// empujarla a la ventana 
int	motor(t_world *world)
{
	t_data *data;
	t_vector screen_vectors[WINDOW_WIDTH];

	data = world->data;

	
	//Verifica cual es la ultima tecla que se pulso (y permanece pulsada)
	//si fue un giro de camara <- o -> 
	// -Actualiza el valor en world.char_direction
	// -Recalcula screen_vectors
	check_loock(world, screen_vectors);


	//Redibujar imagen
	draw_image(world, screen_vectors);
	

	mlx_put_image_to_window(data->mlx, data->window, data->img, 0, 0);
	return (1);

}

// paso template vector como puntero para evitar copias de memoria
calculate_vector(int i, const t_vector *template_vector ,t_vector *screen_vector)
{

	//POR HACER
}

;

void draw_image(t_world *world, t_vector screen_vectors[])
{
	int i;
	double distances[WINDOW_WIDTH];
	t_wall wall[WINDOW_WIDTH];

	i = 0;
	//Recalcular screen_vectors para la nueva direccion del personaje
	// POR HACER: Aqui entraria una buena optimizacion, comprobar si world->char_direction, ha cambiado desde la anterior llamada a esta funcion (con una variable estatica)
	while(i < WINDOW_WIDTH)
	{
		calculate_vector(i, &(world->template_vectors[i]) ,&(screen_vectors[i]));
		i++;
	}
	
	calculate_distances(world, screen_vectors, distances, wall);
	//por hacer:
	//print_column(distances, world->data, world->textures);
}

void calculate_distances(t_world *world, const t_vector vectors[], double distances[], t_wall *wall)
{
	int i;

	i = 0;
	while(i < WINDOW_WIDTH)
	{
		distances[i] = one_ray(world->map, &(world->char_position) , &(vectors[i]), wall[i]);
		i++;
	}
}


void init_ray(t_vector *char_position, t_vector *vector, t_ray *ray)
{
	//En que baldosa del mapa comienza el rayo (su valor se ira incrementando al lo largo del DDA):
	ray->tile_x = (int)char_position->x;
	ray->tile_y = (int)char_position->y;
	//A lo largo del rayo, cual es la distancia entre dos lineas del mismo eje:
	if (vector->x == 0)
		ray->delta_dist_x = MAX_RAY_DISTANCE;
	else
		ray->delta_dist_x = fabs(1 / vector->x);
	if (vector->y == 0)
		ray->delta_dist_y = MAX_RAY_DISTANCE;
	else
		ray->delta_dist_y = fabs(1 / vector->x);
	//Cuanto falta hasta la proxima linea ray->side_dist_ :
	calc_side_dist(ray, char_position, vector);
}

// ray->side_dist_x:Cuanto falta hasta la proxima linea
// ray->step_x:nos mobemos a delante(1) o acia detras(-1) en x
void calc_side_dist(t_ray *ray, t_vector *char_position, t_vector *vector)
{
	if (vector->x > 0)
	{
		ray->step_x = 1;
		ray->side_dist_x = (char_position->x + 1.0 - ray->tile_x) * ray->delta_dist_x; 
	}
	else
	{
		ray->step_x = -1;
		ray->side_dist_x = (char_position->x - ray->tile_x) * ray->delta_dist_x; 
	}
	if (vector->y > 0)
	{
		ray->step_y = 1;
		ray->side_dist_y = (char_position->y + 1.0 - ray->tile_y) * ray->delta_dist_y; 
	}
	else
	{
		ray->step_y = -1;
		ray->side_dist_y = (char_position->y - ray->tile_y) * ray->delta_dist_y; 
	}
}


//OPTIMIZACION: en lugar de pasar t_vector, pasamos (t_vector *) un solo puntero
// es mas economico que copiar una estructura con dos doubles
double	one_ray(const char **map, t_vector *char_position, t_vector *vector, t_wall *wall)
{
	t_ray ray;

	init_ray(char_position, vector, &(ray));
	while(1)
	{
		//NO PROTEJO ACCESO A MEMMORIA INEXISTENTE (mucho lio, y se supone que son mapas cerrados por 1)
		//anque las mates nos podrian llevar a escenarios donde sea un problema. Ya veremos.
		//if rayo colisiona muro:
		if(map[ray.tile_y][ray.tile_x] == 1)
		{
			if(wall == VERTICAL)
				return((ray.tile_x - char_position->x + (1 - ray.step_x) / 2) / vector->x);
			if(wall == HORIZONTAL)
				return((ray.tile_y - char_position->y + (1 - ray.step_y) / 2) / vector->y);
		}
		//incrementamos distancia alternativamente side_dist_x side_dist_y
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
