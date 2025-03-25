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
void    close_fd(int *fd)
{
    if (*fd != -1)
        close(*fd);
    *fd = -1;
}

/* fonction qui ferme tous les descripteurs de fichiers des pipes utilises
pour la communication entre les processus */
void    close_all_pipes(t_pipes *p, int nbr_of_cmds)
{
    int i; // pour parcourir les pipes

    i = 0;
    // boucle sur le tableau de pipes
    while (i < nbr_of_cmds)
    {
        // fermeture de l'extremite de lecture du pipe [0]
        if (p[i].pipefd[0] != -1)
        {
            close_fd(&p[i].pipefd[0]);
            p[i].pipefd[0] = -1;
        }
        // fermeture de l'extremite d'ecriture du pipe [1]
        if (p[i].pipefd[1] != -1)
        {
            close_fd(&p[i].pipefd[1]);
            p[i].pipefd[1] = -1;
        }
        i++;
    }
}

/* fonction pour liberer la memoire allouee dynamiquement d'un tableau
de chaine de caracteres */
void    free_split(char **strs)
{
    int i;

    i = -1;
    while (strs[++i] != NULL)
        free(strs[i]); // elements du tableau
    free(strs); // le tableau lui meme
}

/* fonction qui attend les processus enfants, traite les erreurs liees
aux signaux, et nettoie l'affichage du terminal si necessaire */
void    get_pid_status(int last_process, t_minishell *minishell)
{
    int i;
    int status;

    // attente du processus last_process
    i = minishell->nb_of_cmds - 1;
    // attend que le processus specifie termine. Le statut du processus est stocke dans status
    waitpid(minishell->pids[last_process], &status, 0);
    minishell->exit_code = WEXITSTATUS(status); // macro qui permet de recup le code de sortie d'un processus lorsque celui ci se termine normalement (sans signal)
    if (WIFSIGNALED(status))
    {
        minishell->exit_code = 128 + WTERMSIG(status);
        if (WTERMSIG(status) == SIGQUIT)
            write(2, "Quit (core dumped)\n", 19);
    }
    // attente de tous les autres processus
    while (i >= 0)
    {
        wait(&status);
        if (WIFSIGNALED(status))
            printf("\r\033[2K");
        i--;
    }
}

// !! Une commande dans un shell est executee sous la forme d'un processus !!