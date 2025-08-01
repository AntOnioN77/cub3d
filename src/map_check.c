#include "../inc/cub.h"

int	has_valid_chars(char **map)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			c = map[i][j];
			if (c != '0' && c != '1' && c != 'N'
				&& c != 'S' && c != 'E' && c != 'W'
				&& c != ' ')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

static int	check_vertical(char **map, int i, int j)
{
	int	y;

	y = i;
	while (y >= 0 && map[y][j] != '1')
		y--;
	if (y < 0)
		return (0);
	y = i;
	while (map[++y] && map[y][j] != '1')
		;
	if (!map[y])
		return (0);
	return (1);
}

static int	check_horizontal(char **map, int i, int j)
{
	int	x;

	x = j;
	while (x >= 0 && map[i][x] != '1')
		x--;
	if (x < 0)
		return (0);
	x = j;
	while (map[i][++x] && map[i][x] != '1')
		;
	if (!map[i][x])
		return (0);
	return (1);
}

int	is_map_closed(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == '0' || map[i][j] == 'N'
				|| map[i][j] == 'S' || map[i][j] == 'E'
				|| map[i][j] == 'W')
			{
				if (!check_vertical(map, i, j)
					|| !check_horizontal(map, i, j))
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	has_one_player(char **map)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == 'N' || map[i][j] == 'S'
				|| map[i][j] == 'E' || map[i][j] == 'W')
				count++;
			j++;
		}
		i++;
	}
	return (count == 1);
}
