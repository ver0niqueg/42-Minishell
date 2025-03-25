/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:10:27 by viviane           #+#    #+#             */
/*   Updated: 2025/03/08 18:38:28 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    handle_quote(char *line, char quote, char *to_free)
{
    char    *quote_pos;

    quote_pos = ft_strchr(line, quote);
    if (quote_pos == NULL)
        free(to_free), exit(EXIT_FAILURE);
    handle_first_quote(quote_pos + 1, 1, to_free);
    return ;
}

void    handle_first_quote(char *line, int inside_quote, char *to_free)
{
    char *double_quote;
    char *simple_quote;

    double_quote = NULL;
    simple_quote = NULL;
    if (line == NULL && inside_quote == 1)
        free(to_free), exit(EXIT_FAILURE);
    double_quote = ft_strchr(line, '"');
    simple_quote = ft_strchr(line, '\'');
    if ((double_quote != NULL && simple_quote == NULL) || (double_quote < simple_quote && (double_quote && simple_quote)))
        (handle_quote(double_quote + 1, '"', to_free));
    else if ((double_quote == NULL && simple_quote != NULL) 
                || (double_quote > simple_quote && (double_quote && simple_quote)))
        (handle_quote(simple_quote + 1, '\'', to_free));
    return ;
}
