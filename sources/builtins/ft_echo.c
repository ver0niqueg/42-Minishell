/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:30:19 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/11 16:30:19 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* cmd echo sert a afficher du texte a l'ecran. Elle est souvent utilisee dans 
les scripts shell pour afficher des messages ou le contenu de variables ex : 
echo "Hello, World!" -> Hello, World! */

/* fonction qui detecte si un arg correspond a une option -n */
static bool	check_n(char *arg)
{
	int	i;

	if (arg == NULL || arg[0] != '-')
		return (false);
	if (arg[0] == '-' && arg[1] == '\0')
		return (false);
	i = 1;
	while (arg[i] != '\0')
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

/* fonction qui affiche les arguments d'une cmd echo */
static void	write_args(char **args, int start, t_fd output_fd)
{
	int	i;
	int	already_done;

	i = start;
	already_done = 0;
	while (args[i] != NULL)
	{
		if (already_done == 1)
			write(output_fd, " ", 1);
		write(output_fd, args[i], ft_strlen(args[i]));
		already_done = 1;
		i++;
	}
}

int	ft_echo(char **args, t_fd input_fd, t_fd output_fd, char ***envp)
{
	bool	has_newline;
	int		i;

	(void)input_fd;
	(void)envp;
	has_newline = true;
	i = 0;
	if (args[i] != NULL && check_n(args[i]))
	{
		has_newline = false;
		i++;
	}
	write_args(args, i, output_fd);
	if (has_newline == true)
		write(output_fd, "\n", 1);
	if (errno != 0)
	{
		write_error_msg("minishell: echo: write error: ", strerror(errno));
		return (1);
	}
	return (0);
}
