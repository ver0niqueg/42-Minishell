/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <viviane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:49:08 by viviane           #+#    #+#             */
/*   Updated: 2025/03/27 21:03:38 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Permet détecter et de traiter les redirections dans la ligne de commande.
Elle met a jour la struct t_parsing en fonction des redirections que l'on
va trouver. 
Elle identifie tout d'abord le signe de redirection (< ou >).
Elle  va ensuite sauter les espaces si elle en trouve, pour trouver le début du
fichier ou le délimiteur.
Si le signe est */
void    handle_redir_type(t_line_info line_info, int *limiter_size,
                        t_parsing *parsed, t_minishell *minishell)
{
    char redir;
    int start;

    redir = line_info.line[*line_info.index];
    start = *line_info.index + 1;
    while (line_info.line[start] == ' ')
        start++;
    if (redir == '<' && line_info.line[start] == redir)
        *line_info.index = handle_heredoc(line_info.line, start, parsed, limiter_size);
    if (parsed->err_msg == NULL)
    {
        if (redir == '<' && line_info.line[start] != redir)
            *line_info.index = handle_entry(line_info.line, start,
                                parsed, minishell);
        else if (redir == '>')
            *line_info.index = handle_exit(line_info.line, *line_info.index + 1, 
                                parsed, minishell);
    }
    return ;
}

/*Permet d'extraire une sous chaine à partir de la ligne de commande.
Cette sous chaine prend en compte le critère émis grâce à end_of_word.*/
char    *extract_token(char *line, int *i, t_parsing *parsed)
{
    int end;
    int start;
    char *extract_buffer;

    if (parsed->err_msg != NULL)
        return (NULL);
    extract_buffer = NULL;
    start = *i;
    end = start;
    while (find_end_of_word(&end, line) == 1)
        ft_substring(line + start, end - start - 1, &extract_buffer);
    (*i) = end;
    return (extract_buffer);
}

/*Sert à ajouter la commande traité à la structure parsing, après
avoir traité les signes (guillemets, variables), et effectué une expansion des
variables d'envrionnements.*/
void    add_command(t_line_info line_info, int *cmd_size, t_parsing *parsed, 
                        t_minishell *minishell)
{
    char    *buffer;
    
    buffer = extract_token(line_info.line, line_info.index, parsed);
    if (!buffer)
        return ;
    expand(&buffer, minishell);
    if (!buffer)
        return ;
    if (buffer)
    {
        add_str(&parsed->cmd, buffer, cmd_size);
        free(buffer);
        buffer = NULL;
    }
}

/*Elle permet d'analyser la ligne de commande syntaxiquement correcte, 
vu que l'on a verifier la syntaxe auparavant. On stocke les resultats dans
la struct t_parsing qui fait partie de shell comme l'indique t_minishell.
La boucle parcourt chaque éléments de la ligne et en fonction du caractère
rencontré, elle effectue add_command.
Elle supprime ensuite les guillemets du contenue analysé pour nettoyer la 
ligne.*/
void    parsing(t_parsing *parsed, char *line, t_minishell *minishell, int init)
{
    t_line_info  line_info;
    int cmd_size;
    int limiter_size;

    cmd_size = 1;
    limiter_size = 1;
    line_info.line = line;
    ft_memset(parsed, 0, sizeof(t_parsing));
    line_info.index = &init;
    (*line_info.index)--;
    while (*line_info.index < (int)ft_strlen(line))
    {
        if (line[*line_info.index] == '\'')
            add_command(line_info, &cmd_size, parsed, minishell);
        else if (line[*line_info.index] == '"')
            add_command(line_info, &cmd_size, parsed, minishell);
        else if (line[*line_info.index] != ' ' && line[*line_info.index] != '<'
            && line[*line_info.index] != '>')
            add_command(line_info, &cmd_size, parsed, minishell);
        else if (line[*line_info.index] == '<' || line[*line_info.index] == '>')
            handle_redir_type(line_info, &limiter_size, parsed, minishell);
        (*line_info.index)++;
    }
    clean_quotes(parsed);
    return ;
}