/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 13:29:51 by antofern          #+#    #+#             */
/*   Updated: 2024/02/09 16:47:55 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int				i;

	i = 0;
	while (str[i])
		i++;
	while (i >= 1 && (unsigned char)str[i] != (unsigned char)c)
	{
		i--;
	}
	if ((unsigned char)str[i] == (unsigned char)c)
		return ((char *)&str[i]);
	else
		return (NULL);
}
