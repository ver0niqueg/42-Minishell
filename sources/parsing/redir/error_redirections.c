/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:16:06 by viviane           #+#    #+#             */
/*   Updated: 2025/03/12 21:16:07 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*Permet de valider un fichier d'entrée < pour une redirection
en vérifiant les variables ambiguês, les permissions de lecture, et l'existence
du fichier.
entry est le chemin du fichier d'entrée*/
void	entry_error(char *entry, t_parsing *parsed, t_minishell *minishell)
{
	char	*exp;

	exp = NULL;
	define_exp(&exp, entry, minishell);
	if (entry[0] == '$' && !ft_strchr(entry, '\'') && !ft_strchr(entry, '"'))
		if (amb_redir(entry, exp, parsed))
			return ;
	exp = erase_quote(exp);
	if (not_readable(&exp, parsed))
		return ;
	if (count_chars(exp, '/') > 0)
		if (path_error_en(exp, parsed))
			return ;
	if (access(exp, F_OK) == -1)
		return (found_error(parsed, exp, NULL, ": No such file or directory"));
	return (free(exp));
}

/*Permet de vérifier si le fichier est accessible et valide.
Elle gère les permissions refusées, les chemins invalides et les fichiers
inexistants (avec des repertoires valides).
Exit correspond au nom du fichier.
Tout d'abord si exit contient une VAR on va l'expandre.
On va gérer les quotes en les supprimant pour éviter toutes corruptions.
On va vérifier si exit est un répertoire, si non on va bloquer le chemin.
On va vérifier si les permission d'écriture sont OK (avec access et stat).
Si le fichier existe et est accesible on va vérifier son type (fichier ou repo).
On va vérifier si le chemin contient des sous répertoite inaccessibles.
Si les permissions sont refusées, on va sortir un message d'erreur.
Si le fichier n'existe pas = msg_err
Si tout est bon on va ouvrir le fichier avec open_file. */
void	exit_error(char *exit, t_parsing *parsed, t_minishell *minishell)
{
	struct stat		sb;
	char			*exp;

	define_exp(&exp, exit, minishell);
	if (exit[0] == '$' && !ft_strchr(exit, '\'') && !ft_strchr(exit, '"'))
		if (amb_redir(exit, exp, parsed))
			return ;
	exp = erase_quote(exp);
	if (!exp || always_a_dir(exp, ft_strlen(exp) - 1, parsed))
		return ;
	if (access(exp, W_OK) == 0 && stat(exp, &sb) == 0)
		if (bad_redir_gd_file(exp, parsed, sb))
			return ;
	if (count_chars(exp, '/') > 0)
		if (path_error_ex((int)ft_strlen(exp) - 1, exp, parsed))
			return ;
	if (access(exp, W_OK) != 0 && access(exp, F_OK) == 0)
		return (found_error(parsed, exp, NULL, ": Permission denied"));
	if (!exp[0])
		return (found_error(parsed, exp, NULL, ": No such file or directory"));
	open_file(exp, parsed);
	return (free(exp));
}
