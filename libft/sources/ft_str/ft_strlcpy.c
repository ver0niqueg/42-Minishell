/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <viviane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:56:17 by vihane            #+#    #+#             */
/*   Updated: 2025/03/23 19:28:55 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	while (src[i] && size > 1)
	{
		dst[i] = src[i];
		i++;
		size--;
	}
	if (size > 0)
		dst[i] = '\0';
	return (ft_strlen(src));
}

// size_t	ft_strlcpy(char *dst, const char *src, size_t size)
// {
// 	size_t	len;

// 	len = 0;
// 	if (size == 0)
// 		return (ft_strlen(src));
// 	while (src[len] != '\0' && (len < (size - 1)))
// 	{
// 		dst[len] = src[len];
// 		len++;
// 	}
// 	dst[len] = '\0';
// 	return (ft_strlen(src));
// }
