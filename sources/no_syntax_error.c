/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   if_no_syntax_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:39:11 by viviane           #+#    #+#             */
/*   Updated: 2025/03/12 18:48:34 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    if_no_syntax_error(char *line, t_minishell *minishell)
{
    char *substr_line;
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    k = 0;
    if (minishell->parsed[0].limiter)
        free_words(minishell->parsed[0].limiter);
    while (i < minishell->nb_of_cmds)
    {
        find_one_cmd(&j, line);
        substr_line = ft_strndup(line + k, j - k);
        is_failed_malloc(substr_line);
        parsing(&minishell->parsed[i], substr_line, minishell, 1);
        j++;
        k = j;
        i++;
        free(substr_line);
    }
}