/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <viviane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:02:54 by vihane            #+#    #+#             */
/*   Updated: 2025/03/24 21:27:46 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    is_failed_malloc(char *str)
{
    if (str == NULL)
    {
        printf("Error, malloc failed\n");
        exit(EXIT_FAILURE);
    }
}

void    print_error(char *str)
{
    char *msg;

    if (!str)
        return ;
    msg = ft_strjoin("Minishell :, ", str);
    is_failed_malloc(msg);
    if (!msg)
        exit(EXIT_FAILURE);
    write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
    free(msg);
}