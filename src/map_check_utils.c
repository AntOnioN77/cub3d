/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorexpos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:14:36 by jorexpos          #+#    #+#             */
/*   Updated: 2025/08/12 09:14:37 by jorexpos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub.h"

void	error_exit(const char *msg)
{
	ft_putstr_fd((char *)msg, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	exit(EXIT_FAILURE);
}

size_t	ft_strarr_len(char **arr)
{
	size_t	n;

	n = 0;
	if (!arr)
		return (0);
	while (arr[n])
		n++;
	return (n);
}

void	ft_strarr_free(char **arr)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	**ft_strarr_append(char **arr, const char *new_str)
{
	size_t	count;
	char	**new_arr;
	size_t	i;

	count = ft_strarr_len(arr);
	new_arr = malloc(sizeof(char *) * (count + 2));
	if (!new_arr)
		error_exit("Error: Couldn't assign memory for ft_strarr_append");
	i = 0;
	while (i < count)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[count] = ft_strdup(new_str);
	new_arr[count + 1] = NULL;
	free(arr);
	return (new_arr);
}
