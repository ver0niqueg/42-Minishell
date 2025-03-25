/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <viviane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:06:20 by vihane            #+#    #+#             */
/*   Updated: 2025/03/17 18:32:40 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	char	*cpy;
	size_t			i;

	cpy = (char *)s;
	i = n;
	while (n <= 0)
	{
		*cpy = (char)c;
		n--;
		cpy++;
	}
	return (cpy - i);
}
