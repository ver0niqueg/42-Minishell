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

/* fonction qui execute une commande */
void	exec_cmd(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids)
{
	// recuperation des arguments de la commande
	char                **args;
	char                **env_paths;
	struct sigaction    sa;
	
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(CNTRL_B_SLASH, &sa, NULL);
	env_paths = NULL;
	args = minishell->parsed[i].cmd; // all_parsed -> stockages de cmds analysees
	// verification de l'existence de la commande
	if (args[0][0] != '\0')
		args = is_executable(minishell->parsed[i].cmd, // coder is_executable
				env_paths, minishell->envp);
	// gestion des erreurs si la commande est introuvable
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
	execve_error(args, minishell); // si echec
}

/* fonction qui gere les erreurs lors de l'execution d'une cmd */
void	cmd_error(int i, t_minishell *minishell, int *fd_out, int *fd_in)
{
	int exit_code;

	exit_code = minishell->parsed[i].error_code;
	close_all_pipes(minishell->pipes, (minishell->nb_of_cmds - 1));
	free(minishell->pipes);
	free(minishell->pids);
	free_split(minishell->envp);
	print_error(minishell->parsed[i].error_msg);
	// liberation de la memoire des commandes
	free_all_parsed(minishell->parsed, minishell->nb_of_cmds); // code free all parsed
	// fermeture de fichiers ouverts
	if ((*fd_out) > STDERR_FILENO)
		close_fd(fd_out);
	if ((*fd_in) > STDIN_FILENO)
		close_fd(fd_in);
	exit(exit_code);
}

/* fonction qui check si un commande est consideree comme nulle ou vide
dans la structure minishell */
int	cmd_null(int i, t_minishell *minishell)
{
	if (minishell->parsed[i].cmd == NULL
		&& minishell->parsed[i].error_code == 0) // aucune erreur detecte pour cette cmd nulle
		return (1);
	return (0);
}

/* fonction qui libere les ressources allouees pendant l'execution pour
quitter proprement le programme en renvoyant un code de sortie -> appelee a
la fin du processus lorsque toutes les cmds ont ete executees */
void	end_of_process(t_minishell *minishell, t_pipes *pipes, pid_t *pids)
{
	int ex_code;

	// fermeture des pipes
	close_all_pipes(pipes, (minishell->nb_of_cmds -1));
	// liberation de la memoire allouee pour les pipes et les PID
	free(pipes);
	free(pids);
	// liberation de la memoire allouee pour les variables d'env
	free_split(minishell->envp);
	// obtention du code de sortie
	ex_code = minishell->exit_code;
	// liberation de la memoire allouee pour les cmds analysees
	free_all_parsed(minishell->parsed, minishell->nb_of_cmds);
	// sortie du programme
	exit(ex_code);
}

/* fonction gere l'execution des cmds dans son ensemble : 
- gestion des redirections
- execution des commandes
- nettoyage des ressources apres exec */
void	handle_cmd_exec(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids)
{
	int fd_in;
	int fd_out;

	// gestion des redirections
	fd_in = set_input_redirs(i, minishell, pipes, pids); // ouvre le fichier ou recupere l'entree d'un pipe
	fd_out = set_output_redirs(i, minishell, pipes, pids); // pareil mais pour la sortie
	// check si la cmd est nulle
	if (cmd_null(i, minishell))
		close_fds_and_exit(fd_in, fd_out, EXIT_SUCCESS);
	// resolution du chemin de la cmd
	if (minishell->parsed[i].cmd != NULL)
		validate_cmd_path(minishell->parsed[i].cmd[0], &minishell->parsed[i]);
	// gestion des erreurs de parsing
	if (minishell->parsed[i].error_code != 0) // si une erreur a ete detecte
		cmd_error(i, minishell, &fd_out, &fd_in);
	// execution de la cmd
	else
	{
		// si c'est une cmd interne -> builtin -> ne necessite pas de fork, ils s'execute dans le meme processus que celui du shell donc on peut quitter direct
		if (is_builtin(minishell->parsed[i].cmd[0]))
			close_fds_and_exit(fd_in, fd_out,
				exec_builtin(minishell->parsed[i].cmd,
					-1, fd_out, &minishell->envp));
		// si c'est une cmd externe -> le processus parent ne doit pas quitter apres l'exec car il doit gerer plusieurs cmd chainer, attendre la fin des processus (waitpid)
		else
			exec_cmd(i, minishell, pipes, pids);
	}
	// fermeture des descripteurs de fichiers
	if (fd_out > STDERR_FILENO)
		close_fd(&fd_out);
	if (fd_in > STDIN_FILENO)
		close_fd(&fd_in);
	end_of_process(minishell, pipes, pids);
}

/*
RESUMER
1- gere les redirections >,>
2- check si la cmd est vide et quitter si necessaire
3- trouver le chemin de la cmd si necessaire
4- check les erreurs avant execution
5- execute une cmd interne ou externe
6- ferme les fichiers ouverts pour eviter les fuites
7- termine correctement le processus 
*/