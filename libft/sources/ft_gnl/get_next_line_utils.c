/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihane <vihane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:48:46 by vihane            #+#    #+#             */
/*   Updated: 2024/10/22 14:45:16 by vihane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/get_next_line.h"

size_t	get_strlen(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*get_strjoin(char const *s1, char const *s2)
{
	char	*newstr;
	size_t	i;
	size_t	j;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	newstr = (char *)malloc((get_strlen(s1) + get_strlen(s2)) + 1);
	if (newstr == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		newstr[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		newstr[i + j] = s2[j];
		j++;
	}
	newstr[i + j] = '\0';
	return (newstr);
}

char	*get_substr(const char *str, unsigned int start, size_t len)
{
	char	*sschaine;
	size_t	i;
	size_t	j;
	size_t	str_len;

	str_len = get_strlen(str);
	if (str == NULL)
		return (NULL);
	if (start > str_len)
		return (get_strdup(""));
	if (len > (str_len - start))
		len = (str_len - start);
	sschaine = malloc(sizeof(char) * len + 1);
	if (sschaine == NULL)
		return (NULL);
	j = start;
	i = 0;
	while ((i < len) && j < str_len)
	{
		sschaine[i] = str[j];
		i++;
		j++;
	}
	sschaine[i] = '\0';
	return (sschaine);
}

char	*get_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (unsigned char) c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == (unsigned char) c)
		return ((char *)&s[i]);
	return (NULL);
}

char	*get_strdup(const char *str)
{
	char	*dest;
	int		i;

	i = 0;
	dest = malloc(sizeof(char) * (get_strlen(str) + 1));
	if (dest == NULL)
		return (NULL);
	while (str[i] != '\0')
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
