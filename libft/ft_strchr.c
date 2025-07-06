/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 14:25:23 by antofern          #+#    #+#             */
/*   Updated: 2024/01/31 10:29:57 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int				i;
	unsigned char	uc;

	uc = (unsigned char)c;
	i = 0;
	while (s[i] && (unsigned char)s[i] != uc)
	{
		i++;
	}
	if (s[i] == uc)
		return ((char *)&s[i]);
	else
		return (NULL);
}
