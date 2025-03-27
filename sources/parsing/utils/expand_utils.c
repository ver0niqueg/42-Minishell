/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <viviane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:54:19 by viviane           #+#    #+#             */
/*   Updated: 2025/03/27 21:00:29 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*Permet, en décrementant de revenir à la position de $, de remplacer la 
variable dans la ligne de commande par '\0'*/
void    delete_var(char *line, int *i, int to_dollar, int *new_len)
{
    (*i)--;
    while (line[to_dollar] != '\0')
    {
        line[*i] = line[to_dollar];
        to_dollar++;
        (*i)++;
    }
    line[*i] = '\0';
    (*new_len) = *i;
}

void    new_lenght(char *line, char *value, int *i, int *new_len)
{
    int var_len;

    if (!value)
        return ;
    var_len = (int)ft_strlen(value);
    if (var_len <= (int)ft_strlen(line))
    {
        *new_len = var_len;
        *i = var_len;
    }
    else
    {
        *new_len = (int)ft_strlen(line);
        *i = (int)ft_strlen(line);
    }
}

/*Permet de remplacer une clef (variable $VAR ou $?) dans 
la ligne de commande (*line) par sa valeur (value)
strjoin concatène la valeur avec la partie de la ligne qui suit
la clef, is failed malloc vérifie si l'allocation de mémoire pour new_line a
réussi.
head permet de stocker la ligne qui précède la clef
to_dollar représent l'indice où commence la clef dans la ligne.

strlcpy : on copie une chaine de caractère dans une autre, tout en garantissant
que la destination, ici head auquel on a allouer la place nécessaire, n'overflowed 
pas. to dollar, contient l'indice du commencement de la var donc si la var commence
à 5, head a une place de 5 et donc avec strlcpy on copie le début de la ligne dans
head.*/
void    change_to_value(char *value, char **line, int key_len, int to_dollar)
{
    char *new_line;
    char *head;

    if (!value)
        return ;
    new_line = ft_strjoin(value, (*line) + key_len);
    is_malloc_failed(new_line);
    head = (char *)malloc(sizeof(char) * (to_dollar));
    is_malloc_failed(head);
    ft_strlcpy(head, *line, to_dollar);
    free(*line);
    (*line) = ft_strjoin(head, new_line);
    is_malloc_failed(*line);
    free(head);
    free(new_line);
}

char	*ft_getenv(const char **envp, const char *name)
{
	int		i;
	size_t	key_len;

	if (name == NULL || name[0] == '\0')
		return (NULL);
	i = 0;
	while (envp[i] != NULL)
	{
		key_len = ft_strchr(envp[i], '=') - envp[i];
		if (0 == ft_strncmp(envp[i], name, key_len)
			&& ft_strlen(name) == key_len)
			return (ft_strchr(envp[i], '=') + 1);
		i++;
	}
	return (NULL);
}