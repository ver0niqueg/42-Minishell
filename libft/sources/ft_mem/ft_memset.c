/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:06:20 by vihane            #+#    #+#             */
/*   Updated: 2025/04/02 00:12:08 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	char			*cpy;
	size_t			i;

	cpy = (char *)s;
	i = n;
	while (n != 0)
	{
		*cpy = (char)c;
		n--;
		cpy++;
	}
	return (cpy - i);
}
