/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_test.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:57:04 by viviane           #+#    #+#             */
/*   Updated: 2025/03/15 14:57:05 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

/*Permet de detecter les redirections ambigués, dans le cas ou 
une variable utilisé dans une redirection n'est pas définie ou 
s'étend en une chaine vide ou multiple*/
int	amb_redir(char *src, char *expanded, t_parsing *parsed)
{
	char	*buffer;

	if (expanded == NULL)
	{
		buffer = ft_strjoin(src, ": Ambiguous redirect");
		parsed->err_msg = ft_strdup(buffer);
		free(buffer);
		parsed->err_nb = 1;
		free_redir(parsed);
		return (1);
	}
	return (0);
}

void	path_without_pts(char **no_points, char *exp, int end)
{
	*no_points = ft_strndup(exp, end);
	if ((*no_points) == NULL)
		*no_points = ft_strdup(exp);
	is_malloc_failed((*no_points));
}

/*Permet de vérifier si un chemin donné pour une redirection 
correspond à un répertoit au lieu d'un fichier. Ce qui serait faux.
exp est le chemin à vérifier, en est l'index à la fin du chemin.
On ignore le point, on va ensuite vérifier si il n'y a aps trop de
point et si le chemin se finit bien par /. On ignore les double /.
on extrait le nom du fichier sans le ./ ou / ignorés. */
int	always_a_dir(char *exp, int end, t_parsing *parsed)
{
	int		points;
	char	*no_points;

	points = 0;
	no_points = NULL;
	while (exp[points] && exp[points] == '.')
		points++;
	if (exp[0] && points <= 2 && exp[end] == '/')
	{
		while (exp[points] && exp[points] == '/')
			points++;
		while ((end - 1 >= 0) && exp[end] == '/')
			end--;
		path_without_pts(&no_points, exp + points, end - points);
		if (count_chars(no_points, '.') == 0
			&& count_chars(no_points, '/') == 0)
		{
			parsed->err_msg = ft_strjoin(exp, ": Is a directory");
			(is_malloc_failed(no_points), parsed->err_nb = 1);
			(free_redir(parsed), free(no_points), free(exp));
			return (1);
		}
		free(no_points);
	}
	return (0);
}

/*Permet de vérifier si un fichier de sortie est valide pour une redirection > ou >>
et gère deux cas. Si c'est un dossier = erreur, si le fichier est régulier
ou spécial on fera une ouverture sécurisée.
exp = chemin du fichier.
sb = info sur le fichier.
si fd = 1 c'est le mode ajout donc on va ajouter sinon il est égal à 0 est donc c'est
le mode erase est donc on va erased*/
int	bad_redir_gd_file(char *exp, t_parsing *parsed, struct stat sb)
{
	int	fd;

	if (S_ISDIR(sb.st_mode))
	{
		parsed->err_msg = ft_strjoin(exp, ": Is a directory");
		parsed->err_nb = 1;
		(free_redir(parsed), free(exp));
		return (1);
	}
	if (S_ISREG(sb.st_mode) || S_ISCHR(sb.st_mode))
	{
		if (parsed->ex_append == 1)
			fd = open(exp, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(exp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			(print_error("Open error"), exit(EXIT_FAILURE));
		(close(fd), free(exp));
		return (1);
	}
	return (0);
}

/*Permet de vérifier pas à pas si un chemin de redirection est valide en
testant chaque sous-répertoire.
Elle détécte les dossier non accessible en écriture et les types de
fichier invalide (redirection vers un périphérique).
On prépare la structure chech path avec check path init :
cp.last est la position actuelle dans le chemin, cp.dir_or_fil est le
sous chemin à vérifier, cp.sb sont les info du fichier (stat)
On découpe le chemin en segment, on extrait le sous chemin, et on
vérifie l'accès et le type de fichier. */
int	path_error_ex(int end_of_path, char *exp, t_parsing *parsed)
{
	t_check_path	cp;

	check_path_init(&end_of_path, exp, &cp);
	while (exp[cp.last] && cp.last <= end_of_path)
	{
		end_of_dir(exp, &cp.last);
		if (exp[cp.last] && cp.last <= end_of_path)
		{
			cp.dir_or_fil = ft_substr(exp, 0, cp.last);
			is_malloc_failed(cp.dir_or_fil);
			if (access(cp.dir_or_fil, W_OK) == 0
				&& stat(cp.dir_or_fil, &cp.sb) == 0)
			{
				if (wrong_data_type(&cp, parsed, exp, end_of_path))
					return (1);
			}
			else
				if (not_writable(&cp, parsed, exp, end_of_path))
					return (1);
			free(cp.dir_or_fil);
		}
	}
	return (0);
}

