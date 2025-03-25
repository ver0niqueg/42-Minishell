/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:32:20 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/12 22:32:20 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cd_write_error(const char *s1, const char *s2, const char *s3)
{
	write(STDERR_FILENO, s1, ft_strlen(s1));
	write(STDERR_FILENO, s2, ft_strlen(s2));
	write(STDERR_FILENO, s3, ft_strlen(s3));
	write(STDERR_FILENO, "\n", 1);
}

/* fonction qui garantit qu'on ne ferme que des FD ouverts dynamiquement */
static bool	can_close_fd(int fd)
{
	return (fd > 1
		&& fd != STDIN_FILENO
		&& fd != STDOUT_FILENO
		&& fd != STDERR_FILENO);
} 
// fichier >  sont generalement des fichiers ouverts
// empeche les fermetures sur les sorties en question

static bool	is_valid_int(const char *str)
{
	int	i;

	if (str[0] != '+' && str[0] != '-' && ft_isdigit(str[0]) != 1)
		return (false);
	i = 1;
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) == 0)
			return (false);
		i++;
	}
	return (true);
}

void	close_fds_and_exit(t_fd input_fd, t_fd output_fd, int return_code)
{
	if (can_close_fd(input_fd))
		close(input_fd);
	if (can_close_fd(output_fd))
		close(output_fd);
	exit(return_code);
}
/* fonction qui permet de terminer le programme en cours avec un code de
retour specifique */
int	ft_exit(char **args, t_fd input_fd, t_fd output_fd, char ***envp)
{
	int	nb_args;

	(void)envp;
	nb_args = 0;
	// on compte les arguments
	while (args[nb_args] != NULL)
		nb_args++;
	// affichage de exit\n si output_fd est un terminal
	if (1 == isatty(output_fd)) // check si output_fd est un terminal interactif
		write(output_fd, "exit\n", 5);
	// si aucun argument n'est fourni
	if (nb_args == 0)
		close_fds_and_exit(input_fd, output_fd, EXIT_SUCCESS);
	// si l'argument est un nb valide
	if (is_valid_int(args[0]) == true)
	{
		if (nb_args > 1)
		{
			write(STDERR_FILENO, "minishell: exit: too many arguments\n", 36);
			return (1);
		}
		close_fds_and_exit(input_fd, output_fd, ft_atoi(args[0])); // convertit l'arg en int et quitte avec ce code
	}
	else // cas ou l'argument n'est pas un nb
		(cd_write_error("minishell: exit: ",
				args[0], ": numeric argument required"),
			close_fds_and_exit(input_fd, output_fd, 2));
	close_fds_and_exit(input_fd, output_fd, EXIT_SUCCESS);
	return (0);
}