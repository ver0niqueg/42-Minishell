/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:08:51 by viviane           #+#    #+#             */
/*   Updated: 2025/03/12 21:08:52 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int handle_heredoc(char *line, int start, t_parsing *parsed, int *limiter_size)
{
    int end;
    char *limiter;

    limiter = NULL;
    start++;
    ft_skip_spaces(&start, line);
    end = start;
    while (find_end_of_word(&end, line) == 1)
        ft_substring(line + start, end - start - 1, &limiter);
    if (limiter != NULL)
    {
        add_str(&parsed->limiter, limiter, limiter_size);
        if (parsed->entry != NULL)
        {
            free(parsed->entry);
            parsed->entry = NULL;
        }
        free(limiter);
    }
    return (end);
}

int handle_entry(char *line, int start, t_parsing *parsed, t_minishell *minishell)
{
    int end;

    end = start;
    if (parsed->entry != NULL)
    {
        free(parsed->entry);
        parsed->entry = NULL;
    }
    while (find_end_of_word(&end, line) == 1)
        ft_substring(line + start, end - start - 1, &parsed->entry);
    if (parsed->entry != NULL)
    {
        entry_error(parsed->entry, parsed, minishell);
        expand(&(parsed->entry), minishell);
    }
    return (end);

}

int handle_exit(char *line, int start, t_parsing *parsed, t_minishell *minishell)
{
    int end;
    int on_sign;

    on_sign = start - 1;
    parsed->ex_append = 0;
    if (line[start] == '>')
    {
        start++;
        ft_skip_spaces(&start, line);
        parsed->ex_append = 1;
    }
    ft_skip_spaces(&start, line);
    end = start;
    if (parsed->exit != NULL)
    {
        free(parsed->exit);
        parsed->exit = NULL;
    }
    while (find_end_of_word(&end, line) == 1)
        ft_substring(line + start, end - start - 1, &parsed->exit);
    alloc_or_free(on_sign, line, parsed, minishell);
    return (end);
}
