/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   if_syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:16:55 by viviane           #+#    #+#             */
/*   Updated: 2025/03/13 23:16:56 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char    *put_all(char first, char second, char third)
{
    char *letter;

    letter = malloc(sizeof(char) * 5);
    is_failed_malloc(letter);
    letter[0] = first;
    letter[1] = second;
    letter[2] = third;
    letter[3] = '\'';
    letter[4] = '\0';
    return (letter);
}

void    put_first_and_second(char first, char second, char *err_msg,
                            t_parsing *parsed)
{
    char *letter;

    letter = (char *)malloc(sizeof(char) * 4);
    is_failed_malloc(letter);
    letter[0] = first;
    letter[1] = second;
    letter[2] = '\'';
    letter[3] = '\0';
    parsed->err_msg = ft_strjoin(err_msg, letter);
    free(letter);
}

void    put_first(char first, char *err_msg, t_parsing *parsed)
{
    char *letter;

    letter = (char *)malloc(sizeof(char) * 3);
    is_failed_malloc(letter);
    letter[0] = first;
    letter[1] = '\'';
    letter[2] = '\0';
    parsed->err_msg = ft_strjoin(err_msg, letter);
    free(letter);
}

void    syntax_err_msg(t_parsing *parsed, char first, char second, char third)
{
    char *err_msg;
    char *letter;

    err_msg = ft_strdup("syntax error near unexpected token `");
    is_failed_malloc(err_msg);
    if (first == '\0')
    {
        parsed->err_msg = ft_strjoin(err_msg, "newline\'");
        is_failed_malloc(err_msg);
    }
    else if (first != '\0' && second == '\0')
        put_first(first, err_msg, parsed);
    else if (first != '\0' && second != '\0' && third == '\0')
        put_first_and_second(first, second, err_msg, parsed);
    else if (first != '\0' && second != '\0' && third != '\0')
    {
        letter = put_all(first, second, third);
        parsed->err_msg = ft_strjoin(err_msg, letter);
        free(letter);
    }
    free(err_msg);
    parsed->err_nb = 2;
}