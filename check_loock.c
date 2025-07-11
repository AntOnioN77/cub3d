#include "cub.h"

void check_loock(t_world *world, t_vector screen_vectors[])
{
    if(world->key == 'd')
    {
        //FUCION PENDIENTE
        rotate_vector(world->char_direction, (- PLAYER_ROTATION_STEP));
    }


}