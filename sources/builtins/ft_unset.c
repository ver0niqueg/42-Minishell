/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:36:31 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/12 22:36:31 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* fonction qui permet de supprimer des variables d'env */
int	ft_unset(char **args, t_fd input_fd, t_fd output_fd, char ***envp)
{
	(void)input_fd;
	(void)output_fd;
	while (*args != NULL)
	{
		if ((*args)[0] != '\0' && 1 == ft_unset_env(*envp, *args))
		{
			write_error_msg("minishell: unset: ", strerror(errno));
			return (1);
		}
		args++;
	}
	return (0);
}
