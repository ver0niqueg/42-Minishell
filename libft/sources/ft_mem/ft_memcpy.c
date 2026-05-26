/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <viviane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:15:40 by vihane            #+#    #+#             */
/*   Updated: 2025/03/23 20:25:09 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*cpy_dest;
	char	*cpy_src;

	if (src == NULL && dest == NULL)
		return (NULL);
	cpy_dest = (char *)dest;
	cpy_src = (char *)src;
	while (n != 0)
	{
		*cpy_dest = *cpy_src;
		n--;
		cpy_dest++;
		cpy_src++;
	}
	return (dest);
}
