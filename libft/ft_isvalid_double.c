/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isvalid_double.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:39:07 by antofern          #+#    #+#             */
/*   Updated: 2024/12/11 17:43:22 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	count_side_dot(int *before_dot, int *after_dot, int dot_count)
{
	if (dot_count == 0)
		(*before_dot)++;
	else
		(*after_dot)++;
}

int	count_dot(int *dot_count, int before_dot)
{
	if (*dot_count > 0 || before_dot == 0)
		return (1);
	*dot_count = *dot_count + 1;
	return (0);
}

int	is_valid_double(const char *str)
{
	int	dot_count;
	int	before_dot;
	int	after_dot;
	int	i;

	dot_count = 0;
	before_dot = 0;
	after_dot = 0;
	if (str[0] == '+' || str[0] == '-')
		str++;
	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			count_side_dot(&before_dot, &after_dot, dot_count);
		else if (str[i] == '.' && count_dot(&dot_count, before_dot))
			return (0);
		if (!ft_isdigit(str[i]) && str[i] != '.')
			return (0);
		i++;
	}
	if ((before_dot > 15 || after_dot > 15 || dot_count == 0)
		&& (after_dot > 0 || i == 0))
		return (0);
	return (1);
}
