/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:22:39 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/11 16:22:39 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* pwd (print working directory) affiche le chemin absolu du repertoire de
travail actuel */

int ft_pwd(char **args, t_fd input_fd, t_fd output_fd, char ***envp)
{
    char    pwd[4096]; // tableau de caractere de taille max PATH_MAX qui stock le chemin du repertoire courant

    (void)args;
    (void)input_fd;
    (void)envp;
    // recuperation du repertoire actuel
    if (getcwd(pwd, sizeof(pwd)) == NULL)
    {
        write_error_msg("minishell: pwd: ", strerror(errno));
        return (1);
    }
    write(output_fd, pwd, ft_strlen(pwd));
    write(output_fd, "\n", 1);
    return (0);
}