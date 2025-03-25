/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 18:57:18 by viviane           #+#    #+#             */
/*   Updated: 2025/03/08 18:33:36 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int if_pipe_first(char *line, t_parsing *parsed)
{
    int i;

    i = 0;
    while (line[i] && line[i] == ' ')
        i++;
    if (line[i] == '|')
    {
        parsed->err_msg = ft_strjoin("", "Syntax error near unexpected token '|'");
        parsed->err_nb = 2;
        return (1);
    }
    return (0);
}

void    ft_skip_spaces(int *i, char *line)
{
    while (line[(*i)] && line[(*i)] == ' ')
        (*i)++;
}

int ft_empty_line(char *line)
{
    int i;
    int count;

    i = -1;
    count = 0;
    if (!line)
        return (1);
    while (line[++i])
        if ((line[i] >= 9 && line[i] <= 13) && line[i] != ' ')
            count++;
    if (count == 0)
        return (1);
    return (0);
}

int count_chars(char *line, char c)
{
    int i;
    int count;
    char quote;

    i = 0;
    count = 0;
    if (line == NULL || line[0] == '\0')
        return (0);
    while (i < (int)ft_strlen(line))
    {
        if (line[i] == '\'' || line[i] == '"')
        {
            quote = line[i];
            i++;
            while (line[i] && line[i] != quote)
                i++;
        }
        if (line[i] && line[i] == c)
            count++;
        i++;
    }
    return (count);
}