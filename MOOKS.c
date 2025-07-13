#include "cub.h"
#include <pthread.h>

void secure_pixel_put(t_data *data, int x, int y, int color);



//MOOK solo para pruebas!!
void rotate_direction(t_vector *direction, double angle)
{
    double old_cos = direction->x;
    double old_sin = direction->y;

    direction->x = old_cos * cos(angle) + old_sin * sin(angle);
    direction->y = -old_cos * sin(angle) + old_sin * cos(angle);

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
			secure_pixel_put(img, x, y, 160);
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

    // Repintar el mapa
    for (y = world->map_height - 1; y >= 0; y--)
    {
        for (x = 0; x < world->map_width; x++)
        {
            int color;
            if (world->map[y][x] == '1')
                color = 0xFFFFFF; // Blanco para obstáculos
            else if (world->map[y][x] == '0')
                color = 0x000000; // Negro para espacios transitables
		    else if (world->map[y][x] == ' ')
                color = 0x000000;
            else
                continue; // Ignorar espacios ' '

            int start_x = x * tile_size;
            int start_y = (world->map_height - 1 - y) * tile_size; // Invertir el eje Y
            int end_x = start_x + tile_size;
            int end_y = start_y + tile_size;

            for (int i = start_y; i < end_y; i++)
                for (int j = start_x; j < end_x; j++)
                    secure_pixel_put(&(map_data->data), j, i, color);
        }
    }

    // Dibujar al personaje
    int player_size = tile_size / 2; // Tamaño del personaje
    int player_x = (int)(world->char_position.x * tile_size);
    int player_y = (int)((world->map_height - 1 - world->char_position.y) * tile_size); // Invertir el eje Y
    for (int i = player_y - player_size / 2; i < player_y + player_size / 2; i++)
        for (int j = player_x - player_size / 2; j < player_x + player_size / 2; j++)
           secure_pixel_put(&(map_data->data), j, i, 0xFF0000); // Rojo para el personaje

    // Dibujar las líneas de visión del personaje
    double angle_offset = M_PI / 6; // 60 grados dividido entre 2
    double base_angle = atan2(world->char_direction.y, world->char_direction.x);
    double left_angle = base_angle - angle_offset;
    double right_angle = base_angle + angle_offset;

    double angles[2] = {left_angle, right_angle};

    for (int k = 0; k < 2; k++) // Dibujar dos líneas
    {
        double angle = angles[k];
        double dx = cos(angle);
        double dy = -sin(angle); // Invertir el eje Y

        int x = player_x;
        int y = player_y;

        while (x >= 0 && x < world->map_width * tile_size && y >= 0 && y < world->map_height * tile_size)
        {
            for (int i = -1; i <= 1; i++) // Línea más gruesa
            {
                for (int j = -1; j <= 1; j++)
                {
                    secure_pixel_put(&(map_data->data), x + i, y + j, 0x00FF00);
                }
            }
            x += dx * 8; // Incrementar en pasos de 5 píxeles
            y += dy * 8;
        }
    }
	mlx_put_image_to_window(map_data->data.mlx, map_data->data.window, map_data->data.img, 0, 0);
}

int update_map(void *param)
{
	usleep(10000);
    render_map((t_map_data *)param);
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

void secure_pixel_put(t_data *data, int x, int y, int color)
{
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
    {
        ft_pixel_put(data, x, y, color);
    }
}