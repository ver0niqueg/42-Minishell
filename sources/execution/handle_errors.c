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
	char	*empty; // cmd vide
	char	*to_print; // nom de la cmd qui a echoue
	char	*arg_copy; // copie du premier argument

	empty = "''";
	arg_copy = ft_strdup(args[0]);
	is_malloc_failed(arg_copy);
	// cas ou la cmd est vide
	if (arg_copy[0] == '\0')
		to_print = empty;
	else
		to_print = arg_copy;
	// check du type d'erreur
	if (ft_strchr(to_print, '/') == NULL)
	// si la cmd ne contient pas de /, c'est une cmd simple pas trouvee dans $PATH
	{
		error_msg = ft_strjoin(to_print, ": command not found");
		(is_malloc_failed(error_msg), print_error(error_msg)); // error_msg ?
	}
	// si la cmd contient / mais que le fichier n'existe pas
	else
	{
		error_msg = ft_strjoin(to_print, ": No such file or directory");
		(is_malloc_failed(error_msg), print_error(error_msg));
	}
	(free(error_msg), free(arg_copy));
	minishell->exit_code = 127; // cmd non trouvee
	return ;
}

/* fonction qui gere une erreur liee a dup2() en fermant le descripteur
de fichier concerne */
void	dup_error(int fd)
{
	if (fd >= 0) // si ouvert
		close_fd(&fd); // alors on ferme
	perror("dup2()"); // contient le dernier code d'erreur du systeme
	exit(EXIT_FAILURE);
}

/* fonction gere les erreurs liees a l'appel systeme fork() */
void	fork_error(t_pipes *p, int nbr_of_cmds)
{
	perror("fork()");
	close_all_pipes(p, nbr_of_cmds); // coder close_all_pipes
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

/* fonction pour afficher un message d'erreur */
void	print_error(char *error)
{
	char	*error_msg;

	if (error == NULL)
		return ;
	error_msg = ft_strjoin("minishell: ", error);
	is_malloc_failed(error_msg);
	if (error_msg == NULL)
		exit(EXIT_FAILURE);
	// affichage du message d'erreur
	write(2, error_msg, ft_strlen(error_msg));
	write(2, "\n", 1);
	free(error_msg);
}