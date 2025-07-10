/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:09:58 by antofern          #+#    #+#             */
/*   Updated: 2025/07/10 18:29:03 by antofern         ###   ########.fr       */
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

	i = 0;
	//Recalcular screen_vectors para la nueva direccion del personaje
	// POR HACER: Aqui entraria una buena optimizacion, comprobar si world->char_direction, ha cambiado desde la anterior llamada a esta funcion (con una variable estatica)
	while(i < WINDOW_WIDTH)
	{
		calculate_vector(i, &(world->template_vectors[i]) ,&(screen_vectors[i]));
		i++;
	}
	
	//POR HACER 
	calculate_distances(world, screen_vectors, distances);
	print_column(distances, world->data, world->textures);
}

void calculate_distances(t_world *world, const t_vector vectors[], double distances[])
{
	int i;

	i = 0;
	while(i < WINDOW_WIDTH)
	{
		distances[i] = one_ray(world->map, world->char_position , vectors[i]);
		i++;
	}
}


double	one_ray()
{


	
}