/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:49:10 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/11 16:49:10 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* fonction qui affiche les variables d'env du shell */
int	ft_env(char **args, t_fd input_fd, t_fd output_fd, char ***envp)
{
	int	i;

	(void)args;
	(void)input_fd;
	i = 0;
	while ((*envp)[i] != NULL)
	{
		write(output_fd, (*envp)[i], ft_strlen((*envp)[i]));
		write(output_fd, "\n", 1);
		i++;
	}
	if (errno != 0)
	{
		write_error_msg("minishell: env: write error:", strerror(errno));
		return (125);
	}
	return (0);
}
