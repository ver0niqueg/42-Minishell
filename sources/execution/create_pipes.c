/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:30:37 by vgalmich          #+#    #+#             */
/*   Updated: 2025/02/27 17:30:37 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* fonction qui cree des pipes */
void    create_pipes(t_pipes *p, int nb_pipes)
{
	int i;

	i = 0;
	while (i < nb_pipes)
	{
		if (pipe(p[i].pipefd) == -1) // creation d'un tableau de pipes
		{
			perror("pipe()");
			free(p);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

/* fonction qui gere la creation de processus enfants pour executer des cmds,
la gestion des pipes (redirections entre processus) et la gestion des pids */
void    launch_processes(t_minishell *minishell, int nb_pipes)
{
	int nb_forks;
	int i;

	i = 0;
	nb_forks = minishell->nb_of_cmds;
	// on cree un processus pour chaque commande
	if (nb_pipes > NB_MAX_OF_CMDS - 2)
	{
		nb_pipes = NB_MAX_OF_CMDS - 2;
		nb_forks = NB_MAX_OF_CMDS - 1;
	}
	minishell->pipes = (t_pipes *)malloc(sizeof(t_pipes) * (nb_pipes));
	if (minishell->pipes == NULL)
	{
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		exit(EXIT_FAILURE);
	}
	create_pipes(minishell->pipes, nb_pipes);
	minishell->pids = (pid_t *)malloc(sizeof(pid_t) * (nb_forks));
	if (minishell->pids == NULL)
		pids_alloc_error(minishell->pipes, nb_forks);
	/*
	while(i < nb_forks)
	{
		minishell->pids[i] = -1;
		i++;
	}
	*/
	exec_commands(minishell, nb_forks);
	free(minishell->pids);
}