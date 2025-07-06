//MOOK Main temporal, hasta tener funciones de parseo de mapas.
#include "cub.h"

// Mapa de ejemplo del archivo subject_example.cub (invertido: la última línea es la posición 0)

char* map[] = {
   "11111111 1111111 111111111111    ",
   "11110111 1110101 101111010001    ",
   "11000001110101011111011110N0111  ",
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

int main(void)
{
	t_world *world;
	t_data *data;

	init_data(&data);


	//parser(argc, argv);
	if (0 != init_world(&world))
		return (1);

	mlx_key_hook(data->window, &manage_key, &world);
	mlx_hook(data->window, 17, 0, &close_win, &world);
	mlx_loop(data->mlx);
	return (0);
}

