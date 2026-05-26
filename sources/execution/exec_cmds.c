/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:30:10 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/10 15:30:10 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* fonction qui permet d'executer une cmd interne (builtin) tout en gerant une
eventuelle redirection (append ou truncate) de la sortie vers un fichier */
void	exec_one_builtin(t_minishell *minishell, int *builtin)
{
	int	fd_out;

	fd_out = STDOUT_FILENO;
	if (minishell->parsed[0].exit != NULL)
	{
		if (minishell->parsed[0].ex_append == 1)
			fd_out = open(minishell->parsed[0].exit,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd_out = open(minishell->parsed[0].exit,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_out == -1)
			(print_error("Open error for the only builtin\n"));
	}
	minishell->exit_code = exec_builtin(minishell->parsed[0].cmd,
			-1, fd_out, minishell);
	if (fd_out > STDERR_FILENO)
		close_fd(&fd_out);
	*builtin = 1;
}

/* fonction qui gere une erreur de syntaxe */
void	exit_syntax_error(t_minishell *minishell)
{
	close_all_pipes(minishell->pipes, (minishell->nb_of_cmds - 1));
	free(minishell->pipes);
	free(minishell->pids);
	free_split(minishell->envp);
	free_parsed(minishell->parsed, minishell->nb_of_cmds);
	exit(2);
}

/* fonction qui gere une sortie generale */
void	clean_exit(t_minishell *minishell, int fork_nb, int exit_code, int i)
{
	close_all_pipes(minishell->pipes, (fork_nb - 1));
	free(minishell->pipes);
	free(minishell->pids);
	free_split(minishell->envp);
	print_error(minishell->parsed[i].err_msg);
	free_parsed(minishell->parsed, minishell->nb_of_cmds);
	exit(exit_code);
}

/* fonction qui check si la premiere cmd dans le tableau all_parsed
de la structure minishell est valide */
int	one_valid_cmd(t_minishell *minishell)
{
	if (minishell->parsed[0].cmd != NULL
		&& minishell->parsed[0].err_nb == 0
		&& minishell->nb_of_cmds == 1)
		return (1);
	return (0);
}

/* fonction qui gere l'execution de plusieurs cmds (pipeline) */
void	exec_commands(t_minishell *minishell, int nb_forks)
{
	int	i;
	int	one_builtin;

	i = nb_forks;
	one_builtin = 0;
	if (one_valid_cmd(minishell) && is_builtin(minishell->parsed[0].cmd[0]))
		exec_one_builtin(minishell, &one_builtin);
	while (--i > -1)
	{
		minishell->pids[i] = fork();
		if (minishell->pids[i] == -1)
			fork_error(minishell->pipes, nb_forks);
		else if (minishell->pids[i] == 0)
		{
			if (minishell->parsed[0].err_nb == 2)
				exit_syntax_error(minishell);
			if (one_builtin == 1)
				clean_exit(minishell, nb_forks, minishell->exit_code, i);
			handle_cmd_exec(i, minishell, minishell->pipes, minishell->pids);
		}
	}
	close_all_pipes(minishell->pipes, (nb_forks - 1));
	free(minishell->pipes);
	get_pid_status((nb_forks - 1), minishell);
}
