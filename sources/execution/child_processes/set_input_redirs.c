/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_input_redirs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:29:49 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/14 12:29:49 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/* fonction qui gere l'entree d'un heredoc. Elle cree un pipe, utilise fork()
pour executer un processus enfant qui ecrit dans le pipe, puis le parent 
redirige l'entree standard stdin vers le pipe pour pouvoir lire
le contenu du heredoc. */
void	handle_heredoc_input(int i, t_minishell *minishell, t_pipes *pipes,
		pid_t *pids)
{
	t_pipes	a_pipe;
	pid_t	a_pid;

	if (pipe(a_pipe.pipefd) == -1)
		(perror("pipe()"), exit(EXIT_FAILURE));
	a_pid = fork();
	if (a_pid == 0)
	{
		close_fd(&a_pipe.pipefd[0]);
		if (dup2(a_pipe.pipefd[1], STDOUT_FILENO) == -1)
			close_fd(&a_pipe.pipefd[1]);
		write(1, minishell->parsed[i].limiter_data,
			ft_strlen(minishell->parsed[i].limiter_data));
		close_all_pipes(pipes, (minishell->nb_of_cmds - 1));
		free(pipes);
		free(pids);
		free_split(minishell->envp);
		exit(EXIT_SUCCESS);
	}
	close_fd(&a_pipe.pipefd[1]);
	if (dup2(a_pipe.pipefd[0], STDIN_FILENO) == -1)
		dup_error(a_pipe.pipefd[0]);
	close_fd(&a_pipe.pipefd[0]);
	wait(NULL);
}

/* fonction qui redirige l'entree standard stdin depuis un ficher */
void	redirect_input_from_file(int *fd_in, char *entry)
{
	int	dup_result;

	*fd_in = open(entry, O_RDONLY);
	if (*fd_in == -1)
		print_error("Open entry redirection error");
	dup_result = dup2(*fd_in, STDIN_FILENO);
	if (dup_result == -1)
		dup_error(*fd_in);
	close_fd(fd_in);
}

/* fonction qui redirige l'entree standard d'un processus pour qu'elle
provienne de l'extremite lecture du pipe precedent dans un pipeline */
void	redirect_input_from_pipe(int i, t_minishell *minishell, t_pipes *pipes,
	pid_t *pids)
{
	int	d_result;

	d_result = dup2(minishell->pipes[i - 1].pipefd[0], STDIN_FILENO);
	if (d_result == -1)
	{
		perror("dup2()");
		close_all_pipes(pipes, (minishell->nb_of_cmds -1));
		free(pipes);
		free(pids);
		free_split(minishell->envp);
		exit(EXIT_FAILURE);
	}
	close_fd(&minishell->pipes[i - 1].pipefd[0]);
}

/* fonction qui gere la redirection de l'entree standard stdin depuis :
- un heredoc (<<)
- un fichier (< fichier)
- un pipe (pipeline |)
- terminal (si aucune redirection) */
int	set_input_redirs(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids)
{
	int	j;
	int	fd_in;

	fd_in = STDIN_FILENO;
	if (minishell->parsed[i].limiter_data != NULL
		|| minishell->parsed[i].entry != NULL)
	{
		if (minishell->parsed[i].limiter_data != NULL)
			handle_heredoc_input(i, minishell, pipes, pids);
		else if (minishell->parsed[i].entry != NULL)
			redirect_input_from_file(&fd_in, minishell->parsed[i].entry);
	}
	else
		if (i > 0)
			redirect_input_from_pipe(i, minishell, pipes, pids);
	j = 0;
	while (j < minishell->nb_of_cmds - 1)
	{
		if (j != (i - 1))
			close_fd(&minishell->pipes[j].pipefd[0]);
		j++;
	}
	return (fd_in);
}
