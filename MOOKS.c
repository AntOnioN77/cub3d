#include "cub.h"
#include <pthread.h>


//MOOK solo para pruebas!!
void rotate_direction(t_vector *direction, double angle)
{
    double old_cos = direction->cos;
    double old_sin = direction->sin;

    direction->cos = old_cos * cos(angle) + old_sin * sin(angle);
    direction->sin = -old_cos * sin(angle) + old_sin * cos(angle);
}


//MOOK funcion de testeo de minilibx
void draw_a_square(t_data *img)
{
	int x = 0;
	int y = 0;

	while (y < WINDOW_HEIGHT -1)
	{
		while( x < WINDOW_WIDTH -1)
		{
			ft_pixel_put(img, x, y, 160);
			x++;
		}
		x=0;
		y++;
	}
}

//MOOK Función para ejecutar draw_map en un hilo
void *map_thread(void *arg)
{
    t_world *world = (t_world *)arg;
    draw_map(world);
    return NULL;
}


void render_map(t_map_data *map_data)
{
    t_world *world = map_data->world;
    int tile_size = map_data->tile_size;
    int x, y;

    // Dibujar el mapa
    for (y = world->map_height - 1; y >= 0; y--)
    {
        for (x = 0; x < world->map_width; x++)
        {
            int color = (world->map[y][x] == '1') ? 0xFFFFFF : 0x000000;
            if (world->map[y][x] != ' ')
            {
                int start_x = x * tile_size;
                int start_y = (world->map_height - 1 - y) * tile_size; // Invertir el eje Y
                int end_x = start_x + tile_size;
                int end_y = start_y + tile_size;

                for (int i = start_y; i < end_y; i++)
                    for (int j = start_x; j < end_x; j++)
                        ft_pixel_put(&(map_data->data), j, i, color);
            }
        }
    }

    // Dibujar al personaje
    int player_size = tile_size / 2; // Tamaño del personaje
    int player_x = (int)(world->char_position.x * tile_size);
    int player_y = (int)((world->map_height - 1 - world->char_position.y) * tile_size); // Invertir el eje Y
    for (int i = player_y - player_size / 2; i < player_y + player_size / 2; i++)
        for (int j = player_x - player_size / 2; j < player_x + player_size / 2; j++)
           ft_pixel_put(&(map_data->data), j, i, 0xFF0000); // Rojo para el personaje

    // Dibujar la dirección del personaje
    int dir_length = tile_size; // Longitud de la dirección
    int dir_x = (int)(player_x + world->char_direction.cos * dir_length);
    int dir_y = (int)(player_y - world->char_direction.sin * dir_length); // Invertir el eje Y
    for (int i = -2; i <= 2; i++) // Línea más gruesa
    {
		ft_pixel_put(&(map_data->data), dir_x + i, dir_y, 0x00FF00);
		ft_pixel_put(&(map_data->data), dir_x, dir_y + i, 0x00FF00);
        //mlx_pixel_put(map_data->mlx, map_data->window, dir_x + i, dir_y, 0x00FF00); // Verde para la dirección
        //mlx_pixel_put(map_data->mlx, map_data->window, dir_x, dir_y + i, 0x00FF00);
    }
	mlx_put_image_to_window(map_data->data.mlx, map_data->data.window, map_data->data.img, 0, 0);
}

int update_map(void *param)
{
    render_map((t_map_data *)param);
	usleep(10000);
    return (0);
}

void draw_map(t_world *world)
{
    void *mlx = mlx_init();
    void *window = mlx_new_window(mlx, world->map_width * 20, world->map_height * 20, "Map View");
    t_map_data map_data;
	map_data.world = world;
	map_data.tile_size = 20;
	map_data.data.window = window;
	map_data.data.mlx = mlx;
	map_data.data.img = mlx_new_image(mlx, world->map_width * 20, world->map_height * 20);
	map_data.data.addr = mlx_get_data_addr(map_data.data.img, &(map_data.data.bits_per_pixel), &(map_data.data.line_length), &(map_data.data.endian));
    // Registrar la función de actualización en el bucle principal
    mlx_loop_hook(mlx, update_map, &map_data);

    // Iniciar el bucle de eventos
    mlx_loop(mlx);
}