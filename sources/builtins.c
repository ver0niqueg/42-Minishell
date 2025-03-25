/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:05:54 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/11 15:05:54 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* fonction qui permet de determiner si la commande est un builtin */
bool    is_builtin(const char *cmd)
{
	const char  *builtins[] = \
	{
		"cd",
		"echo",
		"env",
		"exit",
		"export",
		"pwd",
		"unset",
	};
	size_t  nb_builtins;
	size_t  i;

	nb_builtins = sizeof(builtins) / sizeof(*builtins);
	i = 0;
	while (i < nb_builtins)
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}

/* fonction qui gere l'execution d'un builtin qui n'a pas besoin de
l'utilisation de fork = de processus enfant */
int exec_builtin(char **argv, t_fd input_fd, t_fd output_fd, char ***envp)
{
	int result;
	
	result = 0;
	errno = 0;
	if (ft_strcmp(argv[0], "cd") == 0)
		result = ft_cd(argv + 1, input_fd, output_fd, envp);
	else if (ft_strcmp(argv[0], "echo") == 0)
		result = ft_echo(argv + 1, input_fd, output_fd, envp);
	else if (ft_strcmp(argv[0], "env") == 0)
		result = ft_env(argv + 1, input_fd, output_fd, envp);
	else if (ft_strcmp(argv[0], "exit") == 0)
		result = ft_exit(argv + 1, input_fd, output_fd, envp);
	else if (ft_strcmp(argv[0], "export") == 0)
		result = ft_export(argv + 1, input_fd, output_fd, envp);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		result = ft_pwd(argv + 1, input_fd, output_fd, envp);
	else if (ft_strcmp(argv[0], "unset") == 0)
		result = ft_unset(argv + 1, input_fd, output_fd, envp);
	return (result);
}
