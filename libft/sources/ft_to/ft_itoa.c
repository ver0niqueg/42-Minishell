/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <viviane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 18:55:01 by vihane            #+#    #+#             */
/*   Updated: 2025/03/23 19:25:45 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static int	len_size(int n)
{
	int	counter;

	counter = 0;
	if (n < 0)
	{
		counter++;
		n *= -1;
	}
	while (n >= 0)
	{
		n /= 10;
		counter++;
		if (n == 0)
			break ;
	}
	return (counter);
}

char	*n_is_borderline(int n)
{
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	return ((char *) NULL);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		counter;
	int		i;

	i = 1;
	if (n == -2147483648 || n == 0)
		return (n_is_borderline(n));
	counter = len_size(n);
	result = (char *)malloc(sizeof(char) * (counter + 1));
	if (result == NULL)
		return (NULL);
	result[counter] = '\0';
	if (n < 0)
	{
		result[0] = '-';
		n *= -1;
	}
	while (n > 0)
	{
		result[counter - i] = (n % 10) + 48;
		n /= 10;
		i++;
	}
	return (result);
}
