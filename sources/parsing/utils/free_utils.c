/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihane <vihane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:03:43 by viviane           #+#    #+#             */
/*   Updated: 2025/04/02 19:16:37 by vihane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	free_words(char **words)
{
	int	j;

	j = -1;
	if (words == NULL)
		return ;
	while (words[++j])
		free(words[j]);
	free(words);
}

void	free_quotes(char *line)
{
	if (line)
		free(line);
}

void	free_parsed(t_parsing *parsed, int nb_of_cmds)
{
	int	i;

	i = 0;
	while (i < nb_of_cmds)
	{
		if (parsed[i].limiter != NULL)
			free_words(parsed[i].limiter);
		if (parsed[i].cmd != NULL)
			free_words(parsed[i].cmd);
		free(parsed[i].exit);
		free(parsed[i].entry);
		free(parsed[i].limiter_data);
		free(parsed[i].err_msg);
		i++;
	}
	free(parsed);
}

void	free_redir(t_parsing *parsed)
{
	if (parsed->entry)
	{
		free(parsed->entry);
		parsed->entry = NULL;
	}
	if (parsed->exit)
	{
		free(parsed->exit);
		parsed->exit = NULL;
	}
}

void	free_limiters(t_parsing *p, int nb_cmds)
{
	int	i;

	i = 0;
	while (i < nb_cmds)
	{
		free(p[i].limiter_data);
		p[i].limiter_data = NULL;
		i++;
	}
}
