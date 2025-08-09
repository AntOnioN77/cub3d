#include "../inc/cub.h"

static void	load_texture(t_world *w, char *path, void **img, char *err_msg)
{
	t_data	*data;

	data = w->data;
	*img = mlx_xpm_file_to_image(data->mlx, path,
			&w->textures.width, &w->textures.height);
	if (!*img)
		error_exit(err_msg);
}

static void	load_texture_addr(t_world *w, void *img, char **addr)
{
	*addr = mlx_get_data_addr(img,
			&w->textures.bpp, &w->textures.line_length, &w->textures.endian);
}

void	load_textures(t_world *w)
{
	load_texture(w, w->textures.no_texture, &w->textures.no_img,
		"Error: NORTH texture could not be found");
	load_texture(w, w->textures.so_texture, &w->textures.so_img,
		"Error: SOUTH texture could not be found");
	load_texture(w, w->textures.we_texture, &w->textures.we_img,
		"Error: WEST texture could not be found");
	load_texture(w, w->textures.ea_texture, &w->textures.ea_img,
		"Error: EAST texture could not be found");
	load_texture_addr(w, w->textures.no_img, &w->textures.no_addr);
	load_texture_addr(w, w->textures.so_img, &w->textures.so_addr);
	load_texture_addr(w, w->textures.we_img, &w->textures.we_addr);
	load_texture_addr(w, w->textures.ea_img, &w->textures.ea_addr);
}
