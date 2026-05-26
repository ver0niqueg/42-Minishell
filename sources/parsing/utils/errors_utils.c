/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihane <vihane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:02:54 by vihane            #+#    #+#             */
/*   Updated: 2025/04/02 17:48:32 by vihane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	is_malloc_failed(void *str)
{
	if (str == NULL)
	{
		print_error("Error, malloc failed\n");
		exit(EXIT_FAILURE);
	}
}

void	print_error(char *error)
{
	char	*msg;

	if (!error)
		return ;
	msg = ft_strjoin("Minishell: ", error);
	is_malloc_failed(msg);
	if (!msg)
		exit(EXIT_FAILURE);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	free(msg);
}
