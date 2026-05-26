/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:44:52 by viviane           #+#    #+#             */
/*   Updated: 2025/03/08 17:31:53 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*Permet de trouver lq position de la second quote qui ferme la premiere que
l'on trouve (position etant i). On parcout la chaine de caractere a +1 dabord
pour sauter la premiere guillemet et on continue jusquà trouver l'equivalent de
la guillemet appelé en paramètre*/
void	find_second_quote(int *i, char *line, char quote)
{
	int	pos_quote;

	pos_quote = *i + 1;
	while (line[pos_quote] && line[pos_quote] != quote)
		pos_quote++;
	*i = pos_quote;
}

/*Permet de supprimer deux caracteres specifiques, un a la position de i,
l'autre a la position de end.
On supprime et on decale tous les autres char vers la gauche */
char	*erased_str(char *str, int i, int end)
{
	int	len;

	len = ft_strlen(str);
	if (i >= len || end >= len || i < 0 || end < 0)
		return (str);
	ft_memmove(&str[i], &str[i + 1], len - i);
	if (end > i)
		end--;
	ft_memmove(&str[end], &str[end + 1], len - end);
	return (str);
}

/*Permet de supprimer les quotes, mémorise la position
du quote ouvrant, quote est le pointeur qui stocke le
type de quote que l'on recontre.*/
char	*erase_quote(char *str)
{
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	while (str[i] != '\0')
	{
		if ((str[i] && str[i] == '\'') || (str[i] && str[i] == '"'))
		{
			quote = str[i];
			j = i;
			i++;
			while (str[i] && str[i] != quote)
				i++;
			str = reduce_line(str, j, i - 1);
			i -= 1;
		}
		else
			i++;
	}
	return (str);
}

void	clean_quotes(t_parsing *parsed)
{
	int	i;

	i = -1;
	if (parsed->cmd)
		while (parsed->cmd[++i] != NULL)
			parsed->cmd[i] = erase_quote(parsed->cmd[i]);
	i = -1;
	if (parsed->limiter && parsed->limiter != NULL)
	{
		while (parsed->limiter[i++] != NULL)
			parsed->limiter[i] = erase_quote(parsed->limiter[i]);
	}
	parsed->exit = erase_quote(parsed->exit);
	parsed->entry = erase_quote(parsed->entry);
}
