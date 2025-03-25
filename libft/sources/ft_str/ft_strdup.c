/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <viviane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:22:26 by vihane            #+#    #+#             */
/*   Updated: 2025/03/16 18:38:26 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strdup(const char *s)
{
	char	*src;
	int		i;

	i = 0;
	if (s == NULL)
		return (NULL);
	src = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (src == NULL)
		return (NULL);
	while (i < (int)ft_strlen(s))
	{
		{
			src[i] = s[i];
			i++;
		}
	}
	src[i] = '\0';
	return (src);
}
