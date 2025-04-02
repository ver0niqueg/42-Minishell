/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:19:40 by viviane           #+#    #+#             */
/*   Updated: 2025/03/12 19:19:41 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Permet de récupérer la valeur de la variable d'envrionnement,
si elle existe on change la variable pour sa valeur et on réinitialise sa 
longueur pour pouvoir l'afficher et éviter les leaks. Sinon on la supprime*/
void expand_word(char **line, t_point expand, int *new_len, t_minishell *minishell)
{
    char *name;
    char *value;

    (*expand.j)++;
    if (*expand.j >= (int)ft_strlen(*line))
        return;
    while (ft_isalpha((*line)[*expand.j]) || ft_isdigit((*line)[*expand.j]) || (*line)[*expand.j] == '_')
        (*expand.j)++;
    name = ft_strndup((*line) + *expand.i, *expand.j - *expand.i);
    if (name == NULL)
        return;
    value = ft_strdup(ft_getenv((const char **)minishell->envp, name));
    free(name);
    if (value != NULL)
    {
        change_to_value(value, line, *expand.j, *expand.i);
        new_lenght(*line, value, expand.i, new_len);
    }
    else
        delete_var(*line, expand.i, *expand.j, new_len);
    free(value);
}

/*Permet de remplacer $? dans une ligne de commande par le code de sortie de 
la dernière commande exécutée. echo $? affichera 0 si la commande s'est
déroulé avec succès, un autre nombre si elle a échoué. */
void    expand_nbr(char **line, t_point expand, int *new_len, 
                    t_minishell *minishell)
{
    char *value;

    value = NULL;
    (*expand.j)++;
    value = ft_itoa(minishell->exit_code);
    change_to_value(value, line, *expand.j, *expand.i);
    new_lenght((*line), value, expand.i, new_len);
    free(value);
    value = NULL;
}

/*Permet de parcourir la ligne de commande à la recherche d'une variable à
expandre. expand.i marque la position actuelle donc parcourt la ligne.
expand.j marque le début de la variable.
$? affiche le code de la dernière commande donc l'expansion est spécifique.
expand_word permet de traiter les variables d'environnement standard.
Si aucun $ est trouvé on incrémente. */
void    look_for_var(char **line, int *new_len, int *start, t_minishell *minishell)
{
    t_point expand;
    int zero;

    zero = 0;
    expand.i = start;
    expand.j = &zero;
    while ((*line)[*expand.i] && *expand.i <= *new_len)
    {
        if ((*line)[*expand.i] == '$')
        {
            (*expand.i)++;
            *expand.j = *expand.i;
            if ((*line)[*expand.j] == '?')
                expand_nbr(line, expand, new_len, minishell);
            else if (ft_isalpha((*line)[*expand.j]) || (*line)[*expand.j] == '_')
                expand_word(line, expand, new_len, minishell);
        }
        else
            (*expand.i)++;
    }
}

/*Permet d'étendre les variable d'envrionnement si on les trouve entre ""
Si on trouve des '' on saute jusqu'au deuxième. Si on trouve " on avance jusqu'à
trouver le deuxième et on étend la variable.*/
void   expand(char **line, t_minishell *minishell)
{
    int i;
    int new_len;

    i = 0;
    new_len = 0;
    while ((*line) && i < (int)ft_strlen(*line))
    {
        if ((*line)[i] && (*line)[i] == '\'')
            find_second_quote(&i, (*line), '\'');
        else if ((*line)[i] && (*line)[i] != '\'')
        {
            if ((*line)[i] == '"')
            {
                new_len = i;
                i++;
                find_second_quote(&new_len, (*line), '"');
            }
            else
                new_len = i;
            look_for_var(&(*line), &new_len, &i, minishell);
        }
        if ((*line) != NULL && ft_empty_line(*line))
            (free((*line)), (*line) = NULL);
    }
}
