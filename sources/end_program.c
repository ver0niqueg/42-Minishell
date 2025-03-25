/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_program.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:19:11 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/13 00:19:11 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    malloc_failure(void *str)
{
    if (str == NULL)
    {
        print_error("Failed malloc");
        exit(EXIT_FAILURE);
    }
}

void    free_words(char **list)
{
    int j;

    j = -1;
    if (list == NULL)
        return ;
    while (list[++j])
        free(list[j]);
    free(list);
}

void    free_all_parsed(t_parsing *all_parsed, int nb_cmds)
{
    int i;

    i = 0;
    while (i < nb_cmds)
    {
        if (all_parsed[i].limiter != NULL)
            free_words(all_parsed[i].limiter);
        if (all_parsed[i].cmd != NULL)
            free_words(all_parsed[i].cmd);
        free(all_parsed[i].heredoc_data);
        free(all_parsed[i].error_msg);
        free(all_parsed[i].entry);
        free(all_parsed[i].exit);
        i++;
    }
    free(all_parsed);
}