#include "cub.h"

void rotate_normalize_vector(t_vector *vector, double sin, double cos);

void check_loock(t_world *world)
{
    if(world->key == 'd')
    {
        rotate_normalize_vector(&(world->char_direction), sin(PLAYER_ROTATION_STEP), cos(PLAYER_ROTATION_STEP));
		//calculate_camera_plane(world->char_direction.x, world->char_direction.y, &(world->plane_direction));
		rotate_vector(&(world->plane_direction), sin(PLAYER_ROTATION_STEP), cos(PLAYER_ROTATION_STEP));
    }
	else if(world->key == 'a')
	{
	}
	else if(world->key == 'w')
	{
		// Avanzar en la direccion del personaje
		world->char_position.x += world->char_direction.x * PLAYER_MOVE_STEP;
		world->char_position.y += world->char_direction.y * PLAYER_MOVE_STEP;
	}
	else if(world->key == 's')
	{
		// Retroceder en la direccion opuesta del personaje
		world->char_position.x -= world->char_direction.x * PLAYER_MOVE_STEP;
		world->char_position.y -= world->char_direction.y * PLAYER_MOVE_STEP;
	}
if(DEBUGMODE){printf("char_direction == [x%f, y%f]\n", world->char_direction.x, world->char_direction.y);}
if(DEBUGMODE){printf("plane_direction == [x%f, y%f]\n", world->plane_direction.x, world->plane_direction.y);}
}

void rotate_vector(t_vector *vector, double sin, double cos)
{
if(DEBUGMODE2){printf("rotate_vector: vector == [x%f, y%f]\n", vector->x, vector->y);}
	double result_x;
	double result_y;

	result_x = vector->x * cos - vector->y * sin;
	result_y = vector->x * sin + vector->y * cos;
	
	vector->x = result_x;
	vector->y = result_y;
}

/*esta es la formula para rotar en sentido horario un vector
x' = x * cos(θ) - y * sin(θ)
y' = x * sin(θ) + y * cos(θ)*/
void rotate_normalize_vector(t_vector *vector, double sin, double cos)
{
	double result_x;
	double result_y;
	double magnitude;

	result_x = vector->x * cos - vector->y * sin;
	result_y = vector->x * sin + vector->y * cos;
	
	magnitude = sqrt(result_x * result_x + result_y * result_y);
	vector->x = result_x / magnitude;
	vector->y = result_y / magnitude;
}

