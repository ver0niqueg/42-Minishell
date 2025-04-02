/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_test2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 17:20:01 by viviane           #+#    #+#             */
/*   Updated: 2025/03/16 17:20:02 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

/*Permet de vérifier si dans le cas où un fichier existe, 
si il est lisible*/
int	not_readable(char **exp, t_parsing *parsed)
{
	if (access(*exp, F_OK) == 0)
	{
		if (access(*exp, R_OK) == -1)
		{
			parsed->err_msg = ft_strjoin(*exp,
					": Permission denied");
			is_malloc_failed(parsed->err_msg);
			free(*exp);
			parsed->err_nb = 1;
			free_redir(parsed);
			return (1);
		}
	}
	return (0);
}

/*Permet de détecter si un segment du chemin est un fichier régulier alors
qu'un dossier est attendu. Si c'est un fichier régulier alors que ce n'est pas
le dernier segment du chemin, si on attend un dossier, c'est une erreur.
Ex : /usr/local/bin bin est un fichier alor bin : Not a directory */
int	a_file(int end, char *exp, t_parsing *parsed, t_check_path *cp)
{
	if (S_ISREG(cp->sb.st_mode))
		if (!last_word(cp->first, cp->last)
			|| (last_word(cp->first, cp->last) && exp[end] == '/'))
			return (found_error(parsed, exp, cp->dir_or_fil,
					": Not a directory"), 1);
	return (0);
}

/*Permet de détécter l'erreur et d'afficher les messages d'erreurs relatifs au type
de permissions refusées.*/
int	cannot_open(char *exp, t_parsing *parsed, t_check_path *cp)
{
	if (access(exp, F_OK) == 0)
		return (found_error(parsed, exp, cp->dir_or_fil,
				": Permission denied"), 1);
	else
		return (found_error(parsed, exp, cp->dir_or_fil,
				": No such file or directory"), 1);
	return (0);
}

/*Permet de vérifier si le chemin d'entrée est valide en testant 
chaque sous répertoire.
Elle détecte les dossiers illisibles et les fichiers à la place de dossiers.
Elle découpe le chemin en segments, vérifie les autorisations
et si le type est valide, agis si l'accès est réfusé*/
int	path_error_en(char *exp, t_parsing *parsed)
{
	t_check_path	cp;
	int				end;

	end = (int)ft_strlen(exp) - 1;
	check_path_init(&end, exp, &cp);
	put_cursors(exp, end, &cp.first, &cp.last);
	while (cp.last < (int)ft_strlen(exp))
	{
		next_dir(exp, &cp.last, end);
		cp.dir_or_fil = ft_substr(exp, 0, cp.last);
		is_malloc_failed(cp.dir_or_fil);
		if (access(cp.dir_or_fil, R_OK) == 0
			&& stat(cp.dir_or_fil, &cp.sb) == 0)
		{
			if (a_file(end, exp, parsed, &cp))
				return (1);
		}
		else if (access(cp.dir_or_fil, R_OK) != 0)
		{
			if (cannot_open(exp, parsed, &cp))
				return (1);
		}
		(free(cp.dir_or_fil), cp.dir_or_fil = NULL);
	}
	return (0);
}

