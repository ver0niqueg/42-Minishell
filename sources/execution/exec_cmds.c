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
eventuell redirection (append ou truncate) de la sortie vers un fichier */
void    exec_one_builtin(t_minishell *minishell, int *builtin)
{
	int fd_out;

	fd_out = STDOUT_FILENO;
	// check de la redirection de sortie
	if (minishell->parsed[0].exit != NULL)
	{
		// gestion de la redirection de sortie (append ou truncate)
		if (minishell->parsed[0].is_append_mode == 1)
			fd_out = open(minishell->parsed[0].exit,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd_out = open(minishell->parsed[0].exit,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		// check d'erreur lors de l'ouverture du fichier
		if (fd_out == -1)
				(print_error("Open error for the only builtin\n"));
	}
	// execution de la cmd builtin
	minishell->exit_code = exec_builtin(minishell->parsed[0].cmd,
		-1, fd_out, &minishell->envp);
	// fermeture du descripteur de fichier si necessaire
	if (fd_out > STDERR_FILENO)
		close_fd(&fd_out);
	*builtin = 1; // indique que la cmd interne a ete executee
}

/* fonction qui gere une erreur de syntaxe */
void    exit_syntax_error(t_minishell *minishell)
{
	close_all_pipes(minishell->pipes, (minishell->nb_of_cmds - 1));
	free(minishell->pipes);
	free(minishell->pids);
	free_split(minishell->envp);
	free_all_parsed(minishell->parsed, minishell->nb_of_cmds); // check free_all_parsed
	exit(2); // 2 indique generalement une erreur de syntaxe ou erreur fatale pendant l'execution de la cmd
}

/* fonction qui gere une sortie generale */
void    clean_exit(t_minishell *minishell, int fork_nb, int exit_code, int i)
{
	close_all_pipes(minishell->pipes, (fork_nb - 1));
	free(minishell->pipes);
	free(minishell->pids);
	free_split(minishell->envp);
	print_error(minishell->parsed[i].error_msg);
	free_all_parsed(minishell->parsed, minishell->nb_of_cmds); // check free_all_parsed
	exit(exit_code); // 2 indique generalement une erreur de syntaxe ou erreur fatale pendant l'execution de la cmd
}

/* fonction qui check si la premiere cmd dans le tableau all_parsed
de la structure minishell est valide */
int one_valid_cmd(t_minishell *minishell)
{
	if (minishell->parsed[0].cmd != NULL // check si une cmd a ete parsee et stockee dans all_parsed -> l'utilisateur a saisi une cmd valide
		&& minishell->parsed[0].error_code == 0 // check si la cmd parsee n'a pas generee d'erreur
		&& minishell->nb_of_cmds == 1) // check que l'utilisateur n'a entree qu'une seule cmd
		return (1);
	return (0);
}

/* fonction qui gere l'execution de plusieurs cmds */
void	exec_commands(t_minishell *minishell, int nb_forks)
{
	int	i;
	int	one_builtin; // variable pour check si un cmd builtin a ete executee

	i = nb_forks;
	one_builtin = 0;
	if (one_valid_cmd(minishell) && is_builtin(minishell->parsed[0].cmd[0]))
		exec_one_builtin(minishell, &one_builtin); // coder is_builtin
	while (--i > -1)
	{
		minishell->pids[i] = fork(); // creation d'un processus enfant
		if (minishell->pids[i] == -1)
			fork_error(minishell->pipes, nb_forks);
		else if (minishell->pids[i] == 0) // si on est dans le processus enfant car fork() = 0
		{	
			if (minishell->parsed[0].error_code == 2)
				exit_syntax_error(minishell);
			if (one_builtin == 1) // si une cmd builtin a deja ete executee
				clean_exit(minishell, nb_forks, minishell->exit_code, i);
			handle_cmd_exec(i, minishell, minishell->pipes, minishell->pids); // gere l'execution effective
		}
	}
	close_all_pipes(minishell->pipes, (nb_forks - 1));
	free(minishell->pipes);
	get_pid_status((nb_forks - 1), minishell);
}