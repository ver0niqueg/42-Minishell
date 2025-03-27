/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   understand_line.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:08:22 by viviane           #+#    #+#             */
/*   Updated: 2025/03/08 18:01:53 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Cette fonction sert à préparer la structure de données pour stocker les
commandes parsées en comptant le nombre de commandes. Pour cela, on va compter 
le nombre de pipes. 
On initialise la structure minishell en allouant la mémoire nécessaire 
pour stocker les commandes en créant un tableau.
Enfin on initialise tout à 0 grâce à memset avant le parsing détaillé */
void    parse_init_line(char *line, t_minishell *minishell)
{
    minishell->nb_of_cmds = count_chars(line, '|');
    minishell->nb_of_cmds++;
    if (minishell->nb_of_cmds > NB_MAX_OF_CMDS)
    {
        printf("Error, too many commands\n");
        exit(EXIT_FAILURE);
    }
    minishell->parsed = (t_parsing *)malloc(sizeof(t_parsing) * (minishell->nb_of_cmds));
    if (!minishell->parsed)
    {
        printf("Error, malloc failed\n");
        exit(EXIT_FAILURE);
    }
    ft_memset(minishell->parsed, 0, sizeof(t_parsing) * (minishell->nb_of_cmds));
}

/*On parcourt la ligne a la recherche du pipe qui sera le pipe de separation de
commande. Dans le cas ou on trouve u*/
void    find_one_cmd(int *i, char *line)
{
    while (line[*i] != '\0' && line[*i] != '|')
    {
        if (line[*i] == '\'' || line[*i] == '"')
            find_second_quote(i, line, line[*i]);
        (*i)++;
    }
}

/*On analyse la ligne graduellement, d'abord en prenant en compte si les premieres quotes sont bien fermées,
ensuite en initialisant se dont on a besoin et enfin gérant le cas si il y a une erreur de syntaxe ou pas.
Si c'est le cas le code d'erreur 2 sera envoyer à write prompt. */
void understand_line(char *line, t_minishell *minishell)
{
    handle_first_quote(line, 1, line);
    parse_init_line(line, minishell);
    if_syntax_error(line, &minishell->parsed[0]);
    if (minishell->parsed[0].err_nb == 2 && minishell->parsed[0].err_msg)
        minishell->nb_of_cmds = 1;
    else
        if_no_syntax_error(line, minishell);
}