/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atof.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihane <vihane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:52:08 by vihane            #+#    #+#             */
/*   Updated: 2024/11/11 11:55:45 by vihane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static double	parse_fraction(const char *str, int *i)
{
	double	fraction;
	double	divisor;

	fraction = 0.0;
	divisor = 1.0;
	if (str[*i] == '.')
	{
		(*i)++;
		while (ft_isdigit(str[*i]))
		{
			fraction = fraction * 10.0 + (str[(*i)++] - '0');
			divisor *= 10.0;
		}
	}
	return (fraction / divisor);
}

double	ft_atof(const char *str)
{
	double	res;
	double	fraction;
	int		sign;
	int		i;

	res = 0.0;
	sign = 1;
	i = 0;
	if (!str || !*str)
		return (NAN);
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!ft_isdigit(str[i]) && str[i] != '.')
		return (NAN);
	while (ft_isdigit(str[i]))
		res = res * 10.0 + (str[i++] - '0');
	fraction = parse_fraction(str, &i);
	if (str[i])
		return (NAN);
	return (sign * (res + fraction));
}
