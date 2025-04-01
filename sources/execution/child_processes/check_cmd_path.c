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

/* fonction en cas d'erreur dans l'analyse de chemin */
void	cmd_path_error(const char *msg, char *path,
		t_parsing *parsing_data, int nb)
{
	parsing_data->err_msg = ft_strjoin(path, msg);
	parsing_data->err_nb = nb;
	free_redir(parsing_data);
	return ;
}

/* fonction qui check l'existence d'un fichier ou d'un repertoire ainsi que
les permissions necessaires pour y acceder */
int	validate_file_access(t_check_path *cp, char *path, t_parsing *parsing_data)
{
	if (access(cp->dir_or_fil, F_OK) != 0)
		return (cmd_path_error(": No such file or directory",
				path, parsing_data, 127), 1);
	if (access(cp->dir_or_fil, F_OK) == 0
		&& (access(cp->dir_or_fil, R_OK) != 0
			|| access(cp->dir_or_fil, X_OK) != 0))
		return (cmd_path_error(": Permission denied",
				path, parsing_data, 126), 1);
	return (0);
}

/* fonction qui fait la même chose que la precedente mais check en plus si
il s'agit d'un fichier executable ou d'un repertoire  */
int	analyze_path_and_perms(t_check_path *cp,
			char *path, t_parsing *parsing_data)
{
	if (access(cp->dir_or_fil, F_OK) == 0
		&& stat(cp->dir_or_fil, &cp->sb) == 0)
	{
		if (S_ISDIR(cp->sb.st_mode))
		{
			if (last_word(cp->first, cp->last))
				return (cmd_path_error(": Is a directory",
						path, parsing_data, 126), 1);
		}
		else if (S_ISREG(cp->sb.st_mode))
			if (!last_word(cp->first, cp->last)
				|| (last_word(cp->first, cp->last)
					&& path[cp->last] == '/'))
				return (cmd_path_error(": Not a directory",
						path, parsing_data, 126), 1);
	}
	return (validate_file_access(cp, path, parsing_data));
}

/* fonction qui analyse et valide chaque composant d'un chemin de fichier
ou de repertoire dans une chaine, en checkant son existence et ses permissions,
etape necessaire avant l'execution des cmds */
void	validate_cmd_path(char *path, t_parsing *parsing_data)
{
	t_check_path	cp;
	int				return_value;
	int				end;

	return_value = 0;
	end = (int)ft_strlen(path);
	put_cursors(path, end, &cp.first, &cp.last);
	if (count_chars(path, '/') > 0)
	{
		while (cp.last <= end && path[cp.last]
			&& return_value == 0)
		{
			next_dir(path, &cp.last, end);
			if (cp.last > end)
				cp.last = end;
			if (cp.last <= end)
			{
				cp.dir_or_fil = ft_substr(path, 0, cp.last);
				is_malloc_failed(cp.dir_or_fil);
				return_value = analyze_path_and_perms(&cp, path, parsing_data);
				free(cp.dir_or_fil);
			}
		}
	}
}
