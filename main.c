
#include "cub.h"

//MOOK mapa del subject (eje Y inverso para mantener logica cartesiana donde
//la posicion 0 esta abajo y los numeros positivos ascienden)
char* map[] = {
   "11111111 1111111 111111111111    ",
   "11110111 1110101 101111010101    ",
   "1100000111010101111101110000111  ",
   "10000000000000001101010010001    ",
   "10000000000000001100000010011    ",
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
/*
char* map[] = {
   "11111111 1111111 111111111111    ",
   "11110111 1110101 101111011111    ",
   "1100000111010101111101111101111  ",
   "10000000000000001101010011111    ",
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
*/

int main(void)
{
	t_world world;
	t_data *data;

	init_data(&data);
	if (0 != init_world(&world, data))
		return (1);
	mlx_hook(data->window, KEY_PRESS, KEY_PRESS_MASK, press_key, &world);
	mlx_hook(data->window, KEY_RELEASE, KEY_RELEASE_MASK, release_key, &world);
	mlx_hook(data->window, DESTROY_NOTIFY, 0, close_win, &world);
	mlx_loop_hook(data->mlx, motor, &world);
	mlx_loop(data->mlx);
	return (0);
}

//esta funcion deberia leer el archivo con el mapa y rellenar la estructura world->map
//pero por ahora lo dejamos hardcodeado
int init_world(t_world *world, t_data *data)
{
		world->data = data;
		world->textures.ceiling_color = 0x0000FF;// oto//225<<16 | 30<<8 | 0;
		world->textures.floor_color = 220<<16 | 100<<8 | 0;
		world->map = map;
		world->char_position.y = 2.5; // 
		world->char_position.x = 26.5;
		world->char_direction.x = -1;
		world->char_direction.y= 0;
		calculate_camera_plane(world->char_direction.x, world->char_direction.y, &(world->plane_direction)); //perpendicular a char_direction
		world->map_height = 14;
		world->map_width = 33;
		world->textures.wall_color = 0xd2C3D98;
		world->key = 0;
		return (0);
}

//Calcula un vector perpendicular a char_direction, necesario para calcular el angulo
// con que lanzaremos los rayos, evitando el efecto ojo de pez.
void	calculate_camera_plane(double char_dir_x, double char_dir_y, t_vector *plane_direction)
{
	plane_direction->x = -char_dir_y * PLANE_MAGNITUDE;
	plane_direction->y = char_dir_x * PLANE_MAGNITUDE;
}
