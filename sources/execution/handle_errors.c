/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:52:15 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/04 15:52:15 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* fonction qui gere les erreurs lorsqu'une tentative d'execution d'une
commande avec execve echoue */
void	execve_error(char **args, t_minishell *minishell)
{
	char	*error_msg;
	char	*empty;
	char	*to_print;
	char	*arg_copy;

	empty = "''";
	arg_copy = ft_strdup(args[0]);
	is_malloc_failed(arg_copy);
	if (arg_copy[0] == '\0')
		to_print = empty;
	else
		to_print = arg_copy;
	if (ft_strchr(to_print, '/') == NULL)
	{
		error_msg = ft_strjoin(to_print, ": command not found");
		(is_malloc_failed(error_msg), print_error(error_msg));
	}
	else
	{
		error_msg = ft_strjoin(to_print, ": No such file or directory");
		(is_malloc_failed(error_msg), print_error(error_msg));
	}
	(free(error_msg), free(arg_copy));
	minishell->exit_code = 127;
	return ;
}

/* fonction qui gere une erreur liee a dup2() en fermant le descripteur
de fichier concerne */
void	dup_error(int fd)
{
	if (fd >= 0)
		close_fd(&fd);
	perror("dup2()");
	exit(EXIT_FAILURE);
}

/* fonction gere les erreurs liees a l'appel systeme fork() */
void	fork_error(t_pipes *p, int nbr_of_cmds)
{
	perror("fork()");
	close_all_pipes(p, nbr_of_cmds);
	free(p);
	exit(EXIT_FAILURE);
}

/* fonction gere les erreurs d'echec d'allocation memoire */
void	pids_alloc_error(t_pipes *p, int nbr_of_cmds)
{
	perror("malloc()");
	close_all_pipes(p, nbr_of_cmds);
	free(p);
	exit(EXIT_FAILURE);
}
