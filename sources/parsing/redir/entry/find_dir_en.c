/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_dir_en.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 18:13:34 by viviane           #+#    #+#             */
/*   Updated: 2025/03/16 18:13:35 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

/*Ces deux fonctions fonctionnent ensemble pour naviger les segments
d'un chemin. Elle sont utilisées pour analyser les redirections de fichiers.
Celle-ci trouve le dernier segment du chemin avant le fichier final*/
void	put_cursors(char *exp, int end, int *first, int *last)
{
	*first = end - 1;
	while (*first - 1 >= 0 && exp[*first] != '/')
		(*first)--;
	*last = 0;
}

/*Permet d'avancer jusqu'au prochain / pour isoler un segment.*/
void	next_dir(char *exp, int *last, int end)
{
	while (exp[*last] == '/')
		(*last)++;
	if (exp[*last])
		(*last)++;
	while (*last <= end && exp[*last] != '/' )
		(*last)++;
}