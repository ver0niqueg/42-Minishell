/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:22:26 by vihane            #+#    #+#             */
/*   Updated: 2025/04/02 00:12:26 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strdup(const char *s)
{
	char		*cpy;
	size_t		i;

	i = 0;
	if (!s)
		return (NULL);
	cpy = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (cpy == NULL)
		return (NULL);
	while (i < ft_strlen(s))
	{
		cpy[i] = s[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}
