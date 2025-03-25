/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:33:31 by viviane           #+#    #+#             */
/*   Updated: 2025/03/08 17:44:29 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char   *reduce_line(char *line, int start, int end)
{
    while (line[start] != '\0')
    {
        line[start] = line[start + 1];
        start++;
    }
    while (line[end] != '\0')
    {
        line[end] = line[end + 1];
        end++;
    }
    return (line);
}

/*Permet d'ajouter une nouvelle chaine de caractere*/
void    add_str(char ***src_str, char *new_str, int *len)
{
    char **dst_str;
    int i;

    i = 0;
    while ((*src_str) && (*src_str)[i])
        i++;
    dst_str = malloc(sizeof(char *) * (i + 2));
    if (!dst_str)
    {
        printf("Error, malloc failed\n");
        exit(EXIT_FAILURE);
    }
    ft_memcpy(dst_str, *src_str, sizeof(char *) * i);
    dst_str[i] = ft_strdup(new_str);
    if (!dst_str[i])
    {
        printf("Error, malloc failed\n");
        exit(EXIT_FAILURE);
    }
    dst_str[i + 1] = NULL;
    free(*src_str);
    *src_str = dst_str;
    (*len)++;
}

char    *ft_substring(char *line, unsigned int end, char **dest)
{
    if (dest)
        free(*dest);
    *dest = NULL;
    *dest = ft_strndup(line, end + 1);
    is_failed_malloc(*dest);
    return (*dest);
}

void    another_case_of_end(char *line, int *j)
{
    while (line[*j] != '\0' && line[*j] != ' ')
    {
        if (line[*j] == '<' || line[*j] == '>' || line[*j] == '|')
        {
            (*j)--;
            break ;
        }
        if (line[*j] == '\'' && line[*j] == '"')
            find_second_quote(j, line, line[*j]);
        (*j)++;        
    }
}

/*Permet de déterminer la fin d'un mot dans une ligne de commande.
Elle prend en compte les différents delimiteurs ainsi que les guillemets.*/
int find_end_of_word(int *i, char *line)
{
    int start;
    int j;

    start = *i;
    j = start + 1;
    if (line[*i] == ' ' || line[*i] == '\0' || line[*i] == '<'
        || line[*i] == '>' || line[*i] == '|')
        return (*i -= 1, 0);
    if (line[start] == '\'' || line[start] == '\"')
    {
        j = start;
        find_second_quote(&j, line, line[start]);
    }
    else 
        another_case_of_end(line, &j);
    if (!line[j] || line[j] == ' ')
        j--;
    *i = j + 1;
    return (1);
}
