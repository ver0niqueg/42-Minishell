/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihane <vihane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 17:00:24 by vihane            #+#    #+#             */
/*   Updated: 2024/10/22 14:37:55 by vihane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t siz)
{
	size_t	i;
	size_t	j;
	size_t	d_len;
	size_t	s_len;

	d_len = 0;
	s_len = 0;
	while (dst[d_len] != '\0')
		d_len++;
	while (src[s_len] != '\0')
		s_len++;
	if (siz <= d_len)
		return (siz + s_len);
	i = d_len;
	j = 0;
	while ((src[j] != '\0') && (i + 1 < siz))
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (d_len + s_len);
}
