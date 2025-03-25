/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_limiters.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <viviane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 20:23:11 by viviane           #+#    #+#             */
/*   Updated: 2025/03/23 20:48:45 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_strdup_nl(const char *str)
{
	size_t	str_len;
	char	*str_nl;

	str_len = ft_strlen(str);
	str_nl = (char *)malloc(sizeof(char) * (str_len + 1 + 1));
	if (str_nl == NULL)
		return (NULL);
	ft_memcpy(str_nl, str, str_len);
	ft_memcpy(str_nl + str_len, "\n", sizeof(char) * (1 + 1));
	return (str_nl);
}

static char	*add_line_to_lim_data(const char *lim_data, const char *line)
{
	size_t	limdata_len;
	size_t	line_len;
	char	*joined_str;

	if (lim_data == NULL)
		return (ft_strdup_nl(line));
	if (line == NULL)
		return (ft_strdup_nl(lim_data));
	limdata_len = ft_strlen(lim_data);
	line_len = ft_strlen(line);
	joined_str = (char *)malloc(sizeof(char) * (limdata_len + line_len + 2));
	if (joined_str == NULL)
		return (NULL);
	ft_memcpy(joined_str, lim_data, sizeof(char) * limdata_len);
	ft_memcpy(joined_str + limdata_len, line, sizeof(char) * line_len);
	ft_memcpy(joined_str + limdata_len + line_len, "\n", sizeof(char) * 2);
	return (joined_str);
}

static int	ask_limiter(const char *limiter, char **limiter_data,
	bool save_data)
{
	char	*line;
	char	*tmp;

	*limiter_data = NULL;
	while (true)
	{
		line = readline("Cuties$ ");
		if (line == NULL)
			return (-1);
		if (0 == ft_strcmp(limiter, line))
			break ;
		if (save_data)
		{
			tmp = add_line_to_lim_data(*limiter_data, line);
			if (tmp == NULL)
				return ((free(*limiter_data), free(line)), -1);
			(free(*limiter_data), *limiter_data = tmp);
		}
		free(line);
	}
	free(line);
	return (0);
}

int	request_limiters(t_parsing *p, int nb_cmds)
{
	int	i;
	int	j;

	i = 0;
	while (i < nb_cmds)
	{
		j = 0;
		while (p[i].limiter != NULL && p[i].limiter[j] != NULL)
		{
			if (0 != ask_limiter(p[i].limiter[j], &p[i].limiter_data,
					p[i].limiter[j + 1] == NULL))
			{
				free_limiters(p, nb_cmds);
				free_parsed(p, nb_cmds);
				return (-1);
			}
			j++;
		}
		i++;
	}
	return (0);
}
