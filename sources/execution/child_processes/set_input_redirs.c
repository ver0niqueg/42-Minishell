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

/* fonction qui gere la communication entre deux processus via un pipe :
un processus enfant ecrit des donnees dans le pipe, et le processus parent
lit ces donnees a partir du pipe */
void	handle_heredoc_input(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids)
{
	t_pipes a_pipe;
	pid_t   a_pid;

	// creation d'un pipe
	if (pipe(a_pipe.pipefd) == -1)
		(perror("pipe()"), exit(EXIT_FAILURE));
	// creation d'un processus enfant
	a_pid = fork(); // fork cree un nouveau processus enfant qui recoit 0 et le processus parent recoit le pid de l'enfant
	if (a_pid == 0)
	{
		// code du processus enfant
		close_fd(&a_pipe.pipefd[0]); // on ferme la lecture, il va juste ecrire
		if (dup2(a_pipe.pipefd[1], STDOUT_FILENO) == -1) // redirige la sortie standard STDOUT du processus vers le fd du pipe ecriture = tout ce que le processus ecrit sur sa sortie standard ira dans le pipe
			dup_error(a_pipe.pipefd[1]);
		// ensuite on ferme l'ecriture
		close_fd(&a_pipe.pipefd[1]);
		// ecriture des donnes
		write(1, minishell->parsed[i].heredoc_data,
			ft_strlen(minishell->parsed[i].heredoc_data));
		// fermeture de tous les pipes et nettoyage
		close_all_pipes(pipes, (minishell->nb_of_cmds - 1));
		free(pipes);
		free(pids);
		free_split(minishell->envp);
		exit(EXIT_SUCCESS);
	}
	// code du processus parent
	close_fd(&a_pipe.pipefd[1]); // le processus parent n'a pas besoin d'ecrire
	if (dup2(a_pipe.pipefd[0], STDIN_FILENO) == -1) // redirige l'entree standard STDIN du processus parent vers le fd du pipe lecture = le processus parent lira a partir du pipe plutot que l'entree standard
		dup_error(a_pipe.pipefd[0]);
	// fermeture de la lecture du pipe
	close_fd(&a_pipe.pipefd[0]);
	// attendre la fin du processus enfant
	wait(NULL);
}

/*
RESUME:
1- Creation du pipe : un pipe est cree pour permettre la communication entre le processus enfant et le processus parent
2- Processus enfant : redirige la sortie vers le pipe, ecrit des donnees dans le pipe, ferme les fd du pipe et libere les ressources
3- Processus parent : redirige son entree vers le pipe, lit ce que le processus enfant y a ecrit, attend la fin du processus enfant avant de continuer
*/

/* fonction qui redirige l'entree standard depuis un ficher */
void	redirect_input_from_file(int *fd_in, char *entry) // entry -> nom du fichier a ouvrir
{
	int dup_result;

	// ouvrir le fichier en lecture seule
	*fd_in = open(entry, O_RDONLY);
	if (*fd_in == -1)
		print_error("Open entry redirection error");
	// dup2 est une fonction qui duplique un fd, elle redirige l'entree standard STDIN_FILENO vers le fd ouvert precedemment *fd_in
	dup_result = dup2(*fd_in, STDIN_FILENO);
	if (dup_result == -1)
		dup_error(*fd_in);
	close_fd(fd_in);
}

/* fonction qui redirige l'entree standard d'un processus vers (le pipe) la sortie d'un processus precedent dans une chaine de cmds reliees par pipes*/
void	redirect_input_from_pipe(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids)
{
	int	d_result;

	// le but est de rediriger l'entree standard (STDIN_FILENO) pour qu'elle provienne du pipe precedent
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

/* fonction qui gere la redirection de l'entree standard */
int	set_input_redirs(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids)
{
	int j; // compteur pour fermer les fichiers inutiles
	int fd_in;

	fd_in = STDIN_FILENO; // la cmd lira son entree depuis le terminal
	// check de la redirection d'entree
	if (minishell->parsed[i].heredoc_data != NULL // << heredoc
		|| minishell->parsed[i].entry != NULL) // < fichier d'entree
	// si au moins une de ces conditions est vraie, on traite la redirection
	// traitement des redirections
	{
		if (minishell->parsed[i].heredoc_data != NULL)
			handle_heredoc_input(i, minishell, pipes, pids); // la cmd recevra son entree depuis un texte specifie plutot que depuis un fichier ou stdin
		else if (minishell->parsed[i].entry != NULL)
			redirect_input_from_file(&fd_in, minishell->parsed[i].entry); // ouvre le fichie specifie apres < et rediriger fd_in
			// si aucune redirection (ni heredoc ni fichier d'entree)
	}
	else
		if (i > 0) // si ce n'est pas la premiere cmd de pipeline elle doit recup son entree depuis le tube de la cmd precedente
			redirect_input_from_pipe(i, minishell, pipes, pids);
	// fermeture des descripteurs de fichiers inutiles
	j = 0;
	while (j < minishell->nb_of_cmds - 1)
	{
		if (j != (i - 1))
			close_fd(&minishell->pipes[j].pipefd[0]);
		j++;
		// on ferme toutes les entrees des pipes, sauf celle juste avant la cmd actuelle
	}
	return (fd_in);
	// Soit la fonction retourne fd_in qui contient STDIN_FILENO (aucune redirection)
	// soit le descripteurd'un fichier d'entree (< fichier)
	// soit un descripteur de pipe si la commande est dans une pipeline
}