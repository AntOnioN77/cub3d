
#include "cub.h"

//MOOK mapa del subject (eje Y inverso para mantener logica cartesiana donde
//la posicion 0 esta abajo y los numeros positivos ascienden)
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

int init_world(t_world world, t_data **data)
{
		world.data = *data;

		//MOOK valores hardcodeados, para empezar con la logica matematica de raycasting:
		world.textures.ceiling_color = 225<<16 | 30<<8 | 0;
		world.textures.floor_color = 220<<16 | 100<<8 | 0;
		world.map = map;
		world.char_position.y = 2.5; // 
		world.char_position.x = 26.5;
		world.char_direction.y= 1;
		world.char_direction.x = 0;
		world.map_height = 14;
		world.map_width = 33;
		//en caso de error llamar a free_data(*data, 5) talcual esta hardcodeado con valores de ejemplo, el error es imposible

		//Crea la matriz de vectores inicial (centrada en 0.0 , 0.0), a par tir de la cual se calcularan
		//	las subsigientes matrices en funcion de a donde mire el personaje
		init_vectors(world.template_vectors, world.char_direction);

		return (0);
}

int main(void)
{
	t_world world;
	t_data *data;

	init_data(&data);
	if (0 != init_world(world, &data))
		return (1);
	
	//MOOK Crear el hilo para el mapa
	pthread_t map_thread_id;
    if (pthread_create(&map_thread_id, NULL, map_thread, &world) != 0)
    {
        perror("Error al crear el hilo del mapa");
        return (1);
    } //FIN MOOK

	

	mlx_hook(data->window, KEY_PRESS, KEY_PRESS_MASK, press_key, &world);
	mlx_hook(data->window, KEY_RELEASE, KEY_RELEASE_MASK, release_key, &world);
	mlx_hook(data->window, DESTROY_NOTIFY, 0, close_win, &world);
	mlx_loop_hook(data->window, motor, &world);
	mlx_loop(data->mlx);

	pthread_join(map_thread_id, NULL); //BORRAR!!
	return (0);
}

