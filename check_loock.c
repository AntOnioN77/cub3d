#include "cub.h"

void check_loock(t_world *world)
{
    if(world->key == 'd')
    {
        rotate_vector(&(world->char_direction), world->step_rotation, -1);
		rotate_vector(&(world->plane_direction), world->step_rotation, -1);
    }
	else if(world->key == 'a')
	{
		rotate_vector(&(world->char_direction), world->step_rotation, 1);
		rotate_vector(&(world->plane_direction), world->step_rotation, 1);
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
}

void rotate_vector(t_vector *vector, t_vector step_rotation, int direction)
{
	double x;
	double y;
	double magnitude;

	if (direction == 1) // Clockwise
	{
		x = vector->x * step_rotation.y - vector->y * step_rotation.x;
		vector->y = vector->x * step_rotation.x + vector->y * step_rotation.y;
	}
	else if (direction == -1) // Counter-clockwise
	{
		x = vector->x * step_rotation.y + vector->y * step_rotation.x;
		y = -vector->x * step_rotation.x + vector->y * step_rotation.y;
	}
	//normalizar el vector ¿necesario?
	magnitude = sqrt(x*x + y*y);
	vector->x = x / magnitude;
	vector->y = y / magnitude;
}

