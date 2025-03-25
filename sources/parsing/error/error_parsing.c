/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:42:09 by viviane           #+#    #+#             */
/*   Updated: 2025/03/08 18:33:44 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char    *set_limiters(char *line, int *i, t_parsing *parsed)
{
    int end;
    char *buffer;

    buffer = NULL;
    (*i) += 2;
    ft_skip_spaces(i, line);
    if (line[*i] == '|')
        before_pipe(parsed, line, *i);
    else if (line[*i] == '<')
        before_entry(parsed, line, *i);
    else if (line[*i] == '>')
        before_exit(parsed, line, *i);
    else if (line[*i] == '&')
        before_esper(parsed, line, *i);
    else if (line[*i] == '\0')
        syntax_err_msg(parsed, '\0', '\0', '\0');
    else
    {
        end = *i;
        while (find_end_of_word(&end, line) == 1)
            ft_substring(line + (*i), end - (*i) - 1, &buffer);
        *i = end;
    }
    return (buffer);
}

static void	pipes(char *line, int *i, t_parsing *parsed)
{
	(*i)++;
	ft_skip_spaces(i, line);
	if (line[*i] == '|')
		before_pipe(parsed, line, *i);
	else if (line[*i] == '&')
		before_esper(parsed, line, *i);
	else if (line[*i] == '\0')
		syntax_err_msg(parsed, '\0', '\0', '\0');
	if (line[*i] == '<' || line[*i] == '>')
		(*i)--;
}

static void entries_exits(char *line, int *i, t_parsing *parsed)
{
    int end;

    if (line[*i] == '<' && line[(*i) + 1] == '>')
        (*i)++;
    else if (line[*i] == '>' && line[(*i) + 1] == '>')
        (*i)++;
    (*i)++;
    ft_skip_spaces(i, line);
    end = (*i);
    if (line[*i] == '|')
        before_pipe(parsed, line, *i);
    else if (line[*i] == '<')
        before_entry(parsed, line, *i);
    else if (line[*i] == '>')
        before_exit(parsed, line, *i);
    else if (line[*i] == '&')
        before_esper(parsed, line, *i);
    else if (line[*i] == '\0')
        syntax_err_msg(parsed, '\0', '\0', '\0');
    else
    {
        while (find_end_of_word(&end, line) == 1)
            (*i) = end;
    }
}

void    add_limiters(char *buffer, int *limiter_size, t_parsing *parsed)
{
    if (buffer)
    {
        buffer = erase_quote(buffer);
        add_str(&parsed->limiter, buffer, limiter_size);
        free(buffer);
        buffer = NULL;
    }
}

/*Permet de verifier char par char les erreurs de syntaxe qui ne seront donc pas
prise en compte. Elle modifie la struct parsing en ajoutant ou pas les erreurs 
detectees.*/
void    if_syntax_error(char *line, t_parsing *parsed)
{
    int i;
    int limiter_size;

    i = 0;
    limiter_size = 1;
    ft_memset(parsed, 0, sizeof(t_parsing));
    if (if_pipe_first(line, parsed))
        return ;
    while (i < (int)ft_strlen(line) && parsed->err_msg == NULL)
    {
        ft_skip_spaces(&i, line);
        if (line[i] == '\'' || line[i] == '"')
            find_second_quote(&i, line, line[i]);
        if (line[i] == '<' && line[i + 1] == '<')
            add_limiters(set_limiters(line, &i, parsed), &limiter_size, parsed);
        else if (line[i] == '<' || line[i] == '>')
            entries_exits(line, &i, parsed);
        else if (line[i] == '|')
            pipes(line, &i, parsed);
        i++;
    }
    return ;
}