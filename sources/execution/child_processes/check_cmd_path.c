/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:18:26 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/03 18:18:26 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/* Ce fichier analyse et valide le chemin d'une commande en verifiant son
existence, ses permissions et son type (fichier executable ou repertoire)
avant son execution */

/* fonction en cas d'erreur dans l'analyse de chemin */
void	cmd_path_error(const char *msg, char *path, 
				t_parsing *parsing_data, int code)
{
	parsing_data->error_msg = ft_strjoin(path, msg);
	parsing_data->error_code = code;
	free_redir(parsing_data);
	return ;
}

/* fonction qui check l'existence d'un fichier ou d'un repertoire ainsi que
les permissions necessaires pour y acceder */
int	validate_file_access(t_check_path *cp, char *path, t_parsing *parsing_data)
{
	if (access(cp->dir_or_f, F_OK) != 0) // check si le fichier existe
		return (cmd_path_error(": No such file or directory",
				path, parsing_data, 127), 1); // 127 : code d'erreur pour commande introuvable
	if (access(cp->dir_or_f, F_OK) == 0
			&& (access(cp->dir_or_f, R_OK) != 0 // si on ne peut pas le lire
				|| access(cp->dir_or_f, X_OK) != 0)) // si on ne peut pas l'executer
		return (cmd_path_error(": Permission denied",
				path, parsing_data, 126), 1);
	return (0);
}

/* fonction qui fait la même chose que la precedente mais check en plus si
il s'agit d'un fichier executable ou d'un repertoire  */
int	analyze_path_and_perms(t_check_path *cp, 
			char *path, t_parsing *parsing_data)
{
	if (access(cp->dir_or_f, F_OK) == 0
		&& stat(cp->dir_or_f, &cp->sb) == 0) // recup les infos sur le fichier (type, acces, permissions) et les stocke dans sb
	{
		if (S_ISDIR(cp->sb.st_mode)) // si c'est un repertoire
		{
			if (last_word(cp->first, cp->last)) // check si le dernier mot est un repertoire -> ex : /bin/ls YES /bin/ls/ NO
				return (cmd_path_error(": Is a directory",
						path, parsing_data, 126), 1); // 126 : code d'erreur pour permission refusee car ce n'est pas une cmd
		}
		else if (S_ISREG(cp->sb.st_mode)) // si c'est un fichier regulier
			if (!last_word(cp->first, cp->last)
				|| (last_word(cp->first, cp->last)
					&& path[cp->last] == '/'))
				// erreur : ce n'est pas un repertoire, mais il y a une erreur de type de fichier
				return (cmd_path_error(": Not a directory",
						path, parsing_data, 126), 1);
	}
	return (validate_file_access(cp, path, parsing_data)); // check si le fichier est accessible (lecture + execution)
}

/* fonction qui analyse et valide chaque composant d'un chemin de fichier
ou de repertoire dans une chaine, en checkant son existence et ses permissions */
void	validate_cmd_path(char *path, t_parsing *parsing_data)
{
	t_check_path    cp; // structure pour stocker les informations sur le chemin
	int             return_value; // indicateur de retour pour determiner si une erreur a eu lieu ou pas
	int             end; // la longueur de la chaine path

	return_value = 0; // indique aucune erreur
	end = (int)ft_strlen(path);
	put_cursors(path, end, &cp.first, &cp.last); // defini deux pointeurs dans la structures cp -> delimite les parties du chemin
	if (count_chars(path, '/') > 0) // compte le nb de /
	{
		while (cp.last <= end && path[cp.last]
			&& return_value == 0)
		{
			next_dir(path, &cp.last, end); // deplace le curseur a la prochaine position de /
			if (cp.last > end) // si depasse la fin de chaine, alors on reajuste
				cp.last = end;
			if (cp.last <= end) // si la position du curseur est valide
			{
				cp.dir_or_f = ft_substr(path, 0, cp.last); // cela extrait un repertoire ou un fichier du chemin
				malloc_failure(cp.dir_or_f);
				return_value = analyze_path_and_perms(&cp, path, parsing_data); // analyse cette sous chaine extraite pour valider son chemin
				free(cp.dir_or_f); // libere la memoire allouee pour la sous chaine apres analyse
			}
		}
	}
}
