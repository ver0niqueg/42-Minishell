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

#include "../../includes/minishell.h"

/*Cette fonction se manifeste si aucune erreur n'est détéctée. Elle découpe la
ligne de commandes en sous commandes séparés (grâce à strndup) par des pipes.
On lance ensuite le parsing de ces sous commandes une à une.*/
void    if_no_syntax_error(char *line, t_minishell *minishell)
{
    char *substr_line;
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    k = 0;
    if (minishell->parsed[0].limiter != NULL)
        free_words(minishell->parsed[0].limiter);
    while (i < minishell->nb_of_cmds)
    {
        find_one_cmd(&j, line);
        substr_line = ft_strndup(line + k, j - k);
        is_malloc_failed(substr_line);
        parsing(&minishell->parsed[i], substr_line, minishell, 1);
        j++;
        k = j;
        i++;
        free(substr_line);
    }
}