/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_output_redirs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:29:58 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/14 12:29:58 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/* fonction qui redirige la sortie standard d'une cmd vers un pipe */
void	redirect_output_to_next_pipe(t_minishell *minishell, int i, t_pipes *pipes, pid_t *pids)
{
	int dup_result;

	// redirige la sortie standard vers pipe suivant
	dup_result = dup2(minishell->pipes[i].pipefd[1], STDOUT_FILENO);
	if (dup_result == -1)
	{
		perror("dup2()");
		close_all_pipes(pipes, (minishell->nb_of_cmds - 1));
		free(pipes);
		free(pids);
		free_split(minishell->envp);
		exit(EXIT_FAILURE);
	}
	close_fd(&minishell->pipes[i].pipefd[1]);
}

/* fonction qui redirige la sortie vers un fichier -> ecrit dans un fichier */
void	redirect_output_to_file(t_minishell *minishell, int i, int *fd_out)
{
	// check de l'option d'append (ajout) ou de troncature (remplacement)
	if (minishell->parsed[i].ex_append == 1) // ex_append ? 1 en mode ajout non 1 troncature
	// ouverture du fichier de redirection
	// Mode "ajout" append
		*fd_out = open(minishell->parsed[i].exit, 
			O_WRONLY | O_CREAT | O_APPEND, 0644); // ouvre en mode ecriture | si le fichier existe pas il est cree | sortie ajoutee a la fin du fichier sans ecrase le contenu | permissions
	else
	// Mode "troncature" remplacement
		*fd_out = open(minishell->parsed[i].exit, 
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// check de l'ouverture du fichier
	if (*fd_out == -1)
			print_error("Open exit redirection error");
	// redirige le fd (qui fait reference au fichier ouvert) vers la sortie standard (STDOUT_FILENO) -> toute sortie de la cmd sera ecrite dans ce fichier
	if (dup2(*fd_out, STDOUT_FILENO) == -1)
			dup_error(*fd_out);
}

/* fonction qui gere la redirection de la sortie (standard ou vers un fichier)
oiyr ybe commande */
int	set_output_redirs(int i, t_minishell *minishell, t_pipes *pipes, pid_t *pids)
{
	int j;
	int fd_out;

	j = 0; // pour parcourir les fd des pipes
	fd_out = STDOUT_FILENO; // par defaut, la sortie de la cmd ira vers la sortie standard (ecran du terminal)
	// check de la redirection de sortie
	if (minishell->parsed[i].exit == NULL) // si null -> signifie qu'il n'y a pas de redirection de sortie explicite (pas de >, >>)
	{
		if (i < minishell->nb_of_cmds - 1)
			redirect_output_to_next_pipe(minishell, i, pipes, pids);
	}
	else
		redirect_output_to_file(minishell, i, &fd_out);
	// fermeture des descripteurs de fichiers inutiles
	while (j < minishell->nb_of_cmds - 1)
	{
		close_fd(&minishell->pipes[j].pipefd[1]);
		j++;
	}
	return (fd_out);
}