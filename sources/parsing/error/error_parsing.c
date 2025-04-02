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

/*Cette fonction est utilisée pour identifier et extraire des délimiteurs dans
une ligne de commande. On va ignorer les heredoc <<, puis les espaces.
On va ensuite gérer les cas spéciaux d'erreur de syntaxe avec < qui pourrait 
être associé à |, <, >, et &.
On détermine ensuite ou le délimiteur se termine (en sautant les quotes si 
nécessaire) et on isole le délimiteur dans le buffer pour ensuite le retourner,
pour ensuite le nettoyer grâce à add_limiter*/
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

/*Fonction qui permet d'analyser la ligne de commande et de gérer les pipes,
et de détécter les erreurs de syntaxes associées. Dans le cas ou après 
les ESPACES on trouve un pipe, une esperluette, ou la find de la ligne. 
Un message d'erreur s'affiche. On gère aussi le cas de si on trouve une redirection
après le pipe | >; on va reculer i pour retomber sur la redirection et pour que 
la fonction entries_exit puisse la gérer.*/
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

/*Fonction qui permet d'analyser la ligne de commande et de gérer les redirections,
et de détécter les erreurs de syntaxes associées. Les cas traités sont (<>, 
les redirections bidirectionnelle); les redirections avec ajouts (>>); et les
redirections simples. Dans le cas ou après les ESPACES on trouve un pipe, une
redirections simples, une esperluette, ou la find de la ligne. Un message d'erreur
s'affiche. Sinon, on trouve la fin du mot (en n'oubliant pas les quotes et les espaces
etc...). et i pointe a la fin du fichier*/
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

/*Sert à stocker les délimiteurs dans la structure parsing, pour pouvoir 
gérer les redirections comme heredocument (<<). On supprime les quotes autour du
delimiteur ex : cat << "EOF" ici les guillemets disparaissent et on stocke EOF dans
parsed->limiter. Les heredocuments ont besoin de délimiteurs sans quotes pour fonctionner*/
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

/*Cette fonction vérifie les erreurs de syntaxe dans la ligne de commande.
Les pipes mal placés, les redirections incorrectes et les guillemets non fermés.
On vérifie char par char et si une erreur est trouvé err_nb est set à 2.

On réinitialise t_parsing, on vérifie si un pipe est trouvé en première position,
On parcourt la ligne à la recherche d'une erreur (err_msg != NULL) ou la fin de la
ligne.
On commence par skip les espaces en début de ligne si il y en a, 
*/
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