/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 23:05:29 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/05 23:05:29 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/* fonction qui verifie si une cmd existe l'execute */
void	exec_cmd(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids)
{
	char				**args;
	char				**env_paths;
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(CNTRL_B_SLASH, &sa, NULL);
	env_paths = NULL;
	args = minishell->parsed[i].cmd;
	if (args[0][0] != '\0')
		args = is_executable(minishell->parsed[i].cmd,
				env_paths, minishell->envp);
	if (args == NULL)
	{
		perror("env paths");
		close_all_pipes(pipes, (minishell->nb_of_cmds - 1));
		free(pipes);
		free(pids);
		free_split(minishell->envp);
		minishell->exit_code = EXIT_FAILURE;
		return ;
	}
	execve(args[0], args, minishell->envp);
	execve_error(args, minishell);
}

/* fonction qui gere les erreurs lors de l'execution d'une cmd */
void	cmd_error(int i, t_minishell *minishell, int *fd_out, int *fd_in)
{
	int	exit_code;

	exit_code = minishell->parsed[i].err_nb;
	close_all_pipes(minishell->pipes, (minishell->nb_of_cmds - 1));
	free(minishell->pipes);
	free(minishell->pids);
	free_split(minishell->envp);
	print_error(minishell->parsed[i].err_msg);
	free_parsed(minishell->parsed, minishell->nb_of_cmds);
	if ((*fd_out) > STDERR_FILENO)
		close_fd(fd_out);
	if ((*fd_in) > STDIN_FILENO)
		close_fd(fd_in);
	exit(exit_code);
}

/* fonction qui check si un commande est consideree comme nulle ou vide */
int	cmd_null(int i, t_minishell *minishell)
{
	if (minishell->parsed[i].cmd == NULL
		&& minishell->parsed[i].err_nb == 0)
		return (1);
	return (0);
}

/* fonction qui libere les ressources allouees pendant l'execution pour
quitter proprement le programme en renvoyant un code de sortie -> appelee a
la fin du processus lorsque toutes les cmds ont ete executees */
void	end_of_process(t_minishell *minishell, t_pipes *pipes, pid_t *pids)
{
	int	ex_code;

	close_all_pipes(pipes, (minishell->nb_of_cmds -1));
	free(pipes);
	free(pids);
	free_split(minishell->envp);
	ex_code = minishell->exit_code;
	free_parsed(minishell->parsed, minishell->nb_of_cmds);
	exit(ex_code);
}

/* fonction qui gere l'execution des cmds dans son ensemble : 
- gestion des redirections
- execution des commandes
- nettoyage des ressources apres exec */
void	handle_cmd_exec(int i, t_minishell *minishell, t_pipes *pipes,
	pid_t *pids)
{
	int	fd_in;
	int	fd_out;

	fd_in = set_input_redirs(i, minishell, pipes, pids);
	fd_out = set_output_redirs(i, minishell, pipes, pids);
	if (cmd_null(i, minishell))
		close_fds_and_exit(fd_in, fd_out, EXIT_SUCCESS, minishell);
	if (minishell->parsed[i].cmd != NULL)
		validate_cmd_path(minishell->parsed[i].cmd[0], &minishell->parsed[i]);
	if (minishell->parsed[i].err_nb != 0)
		cmd_error(i, minishell, &fd_out, &fd_in);
	else
	{
		if (is_builtin(minishell->parsed[i].cmd[0]))
			close_fds_and_exit(fd_in, fd_out,
				exec_builtin(minishell->parsed[i].cmd,
					-1, fd_out, minishell), minishell);
		else
			exec_cmd(i, minishell, pipes, pids);
	}
	if (fd_out > STDERR_FILENO)
		close_fd(&fd_out);
	if (fd_in > STDIN_FILENO)
		close_fd(&fd_in);
	end_of_process(minishell, pipes, pids);
}
