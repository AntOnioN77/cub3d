/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbr_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 16:32:48 by antofern          #+#    #+#             */
/*   Updated: 2024/07/30 16:32:52 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <limits.h>

static int	ft_is_base_ko(char *base)
{
	int	i;
	int	j;

	i = 0;
	while (base[i] != '\0')
	{
		if (base[i] == '-' || base[i] == '+')
			return (1);
		j = i + 1;
		while (base[j])
		{
			if (base[j] == base[i])
				return (1);
			j++;
		}
		i++;
	}
	if (i < 2)
		return (1);
	return (0);
}

static void	ft_imprime(char *base, int *buf_reminds, int nlength)
{
	int	r;

	while (nlength >= 0)
	{
		r = buf_reminds[nlength];
		write(1, &base[r], 1);
		nlength--;
	}
}

static int	distribute(int *reminds, long unsigned int *unbr,
unsigned int len_base)
{
	int	ndigits;

	ndigits = 0;
	while (len_base <= *unbr)
	{
		reminds[ndigits] = *unbr % len_base;
		*unbr = *unbr / len_base;
		ndigits++;
	}
	ndigits--;
	return (ndigits);
}

void	ft_putunbr_base(long unsigned int nbr, char *base)
{
	unsigned int	len_base;
	int				buf_reminds[32];
	int				nlength;

	len_base = 0;
	while (base[len_base])
		len_base++;
	if (ft_is_base_ko(base) == 1)
		return ;
	nlength = distribute(buf_reminds, &nbr, len_base);
	write(1, &base[nbr], 1);
	ft_imprime(base, buf_reminds, nlength);
}
