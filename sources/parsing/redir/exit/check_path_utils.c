/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 18:01:34 by viviane           #+#    #+#             */
/*   Updated: 2025/03/16 18:01:35 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

/*Permet d'initialiser la structure checkpath*/
void	check_path_init(int *end, char *exp, t_check_path *cp)
{
	cp->counter = 0;
	cp->last = 0;
	while ((*end) - 1 >= 0 && exp[(*end) - 1] == '/')
		(*end)--;
	cp->first = (*end);
	at_last_dir(&cp->first, exp);
}

/*Permet de vérifier si un segment d'un chemin est du bon type
pour une redirection. Elle détecte deux erreurs :
un dossier alors que l'on attend un fichier;
un fichier alors que l'on attend un dossier*/
int	wrong_data_type(t_check_path *cp, t_parsing *parsed, char *exp, int end)
{
	if (S_ISDIR(cp->sb.st_mode))
		if (last_word(cp->first, cp->last) && exp[end] == '/')
			return (found_error(parsed, exp, cp->dir_or_fil,
					": Is a directory"), 1);
	if (S_ISREG(cp->sb.st_mode) && !last_word(cp->first, cp->last))
		return (found_error(parsed, exp, cp->dir_or_fil, ": Not a directory"),
			1);
	return (0);
}

/*Permet de vérifier si un segment de chemin est accessible en
écriture et détecte trois erreurs :
permissions	refusées;
fichier		inexistant;
dossier inattendu (quand un fichier est attendu)*/
int	not_writable(t_check_path *cp, t_parsing *parsed, char *exp,
		int end_of_file)
{
	if (access(cp->dir_or_fil, W_OK) != 0 && access(cp->dir_or_fil, F_OK) == 0)
		return (found_error(parsed, exp, cp->dir_or_fil, ": Permission denied"),
			1);
	else if (access(cp->dir_or_fil, F_OK) != 0 && cp->last <= cp->first)
		return (found_error(parsed, exp, cp->dir_or_fil,
				": No such file or directory"), 1);
	if (last_word(cp->first, cp->last) && exp[end_of_file] == '/')
		return (found_error(parsed, exp, cp->dir_or_fil, ": Is a directory"),
			1);
	return (0);
}
