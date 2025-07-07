#include "cub.h"

// MOOK Mapa de ejemplo del archivo subject_example.cub (invertido: la última línea es la posición 0)
char* map[] = {
   "11111111 1111111 111111111111    ",
   "11110111 1110101 101111010001    ",
   "1100000111010101111101111000111  ",
   "10000000000000001101010010001    ",
   "10000000000000001100000010001    ",
   "11000000110101011100000010001    ",
   "11110111111111011101010010001    ",
   "11110111111111011100000010001    ",
   "100000000011000001110111111111111",
   "111111111011000001110000000000001",
   "        1001000000000000000000001",
   "        1011000001110000000000001",
   "        1000000000110000000000001",
   "        1111111111111111111111111"
};

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

//MOOK mostrar mapa y posicion/direccion del personaje
void draw_map(t_world *world)
{
    void	*mlx;
    void	*window;
    int		tile_size = 20;
    int		x, y;

    mlx = mlx_init();
    window = mlx_new_window(mlx, world->map_width * tile_size, world->map_height * tile_size, "Map View");

    y = 0;
    while (y < world->map_height)
    {
        x = 0;
        while (x < world->map_width)
        {
            int color = (world->map[y][x] == '1') ? 0xFFFFFF : 0x000000;
            if (world->map[y][x] != ' ')
            {
                int start_x = x * tile_size;
                int start_y = y * tile_size;
                int end_x = start_x + tile_size;
                int end_y = start_y + tile_size;

                for (int i = start_y; i < end_y; i++)
                    for (int j = start_x; j < end_x; j++)
                        mlx_pixel_put(mlx, window, j, i, color);
            }
            x++;
        }
        y++;
    }

    //Dibujar al personaje
    int player_x = (int)(world->char_position.x * tile_size);
    int player_y = (int)(world->char_position.y * tile_size);
    mlx_pixel_put(mlx, window, player_x, player_y, 0xFF0000);

    //Dibujar la dirección del personaje
    int dir_x = (int)(player_x + world->char_direction.cos * tile_size);
    int dir_y = (int)(player_y + world->char_direction.sin * tile_size);
    mlx_pixel_put(mlx, window, dir_x, dir_y, 0x00FF00);

    mlx_loop(mlx);
}

//MOOK Función para ejecutar draw_map en un hilo
#include <pthread.h>
void *map_thread(void *arg)
{
    t_world *world = (t_world *)arg;
    draw_map(world);
    return NULL;
}



int init_world(t_world *world, t_data **data)
{
		world->data = *data;

		//MOOK valores hardcodeados, para empezar con la logica matematica de raycasting:
		world->ceiling_color = 225<<16 | 30<<8 | 0;
		world->floor_color = 220<<16 | 100<<8 | 0;
		world->map = map;
		world->char_position.y = 2.0; 
		world->char_position.x = 26;
		world->char_direction.sin= 1;
		world->char_direction.cos = 0;
		world->map_height = 14;
		world->map_width = 33;
		return (0);
}

int main(void)
{
	t_world world;
	t_data *data;

	init_data(&data);
	if (0 != init_world(&world, &data))
		return (1);
	
	//MOOK Crear el hilo para el mapa
	pthread_t map_thread_id;
    if (pthread_create(&map_thread_id, NULL, map_thread, &world) != 0)
    {
        perror("Error al crear el hilo del mapa");
        return (1);
    } //FIN MOOK

	draw_a_square(data);
	mlx_put_image_to_window(data->mlx, data->window, data->img, 0, 0);

	mlx_key_hook(data->window, &manage_key, &world);
	mlx_hook(data->window, 17, 0, &close_win, &world);
	mlx_loop(data->mlx);

	pthread_join(map_thread_id, NULL); //BORRAR!!
	return (0);
}

