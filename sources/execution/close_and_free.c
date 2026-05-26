/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_and_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 23:42:15 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/04 23:42:15 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* fonction qui ferme un descripteur de fichier */
void	close_fd(int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = -1;
}

/* fonction qui ferme tous les fd des pipes utilises pour la communication entre
les processus */
void	close_all_pipes(t_pipes *p, int nbr_of_cmds)
{
	int	i;

	i = 0;
	while (i < nbr_of_cmds)
	{
		if (p[i].pipefd[0] != -1)
		{
			close_fd(&p[i].pipefd[0]);
			p[i].pipefd[0] = -1;
		}
		if (p[i].pipefd[1] != -1)
		{
			close_fd(&p[i].pipefd[1]);
			p[i].pipefd[1] = -1;
		}
		i++;
	}
}

/* fonction pour liberer la memoire d'un tableau de strs */
void	free_split(char **strs)
{
	int	i;

	i = -1;
	while (strs[++i] != NULL)
		free(strs[i]);
	free(strs);
}

/* fonction qui attend les processus enfants, traite les erreurs liees
aux signaux, et nettoie l'affichage du terminal si necessaire */
void	get_pid_status(int last_process, t_minishell *minishell)
{
	int	i;
	int	status;

	i = minishell->nb_of_cmds - 1;
	waitpid(minishell->pids[last_process], &status, 0);
	minishell->exit_code = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		minishell->exit_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(2, "Quit (core dumped)\n", 19);
	}
	while (i >= 0)
	{
		wait(&status);
		if (WIFSIGNALED(status))
			printf("\r\033[2K");
		i--;
	}
}
