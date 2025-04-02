/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_err_msg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 18:37:26 by viviane           #+#    #+#             */
/*   Updated: 2025/03/08 18:37:27 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	also_first(char first, char *msg, t_parsing *parsed)
{
	char	*letters;

	letters = (char *)malloc(sizeof(char) * 3);
	is_malloc_failed(letters);
	letters[0] = first;
	letters[1] = '\'';
	letters[2] = '\0';
	parsed->err_msg = ft_strjoin(msg, letters);
	free(letters);
}

void	also_first_and_second(char first, char second, char *msg,
		t_parsing *parsed)
{
	char	*letters;

	letters = (char *)malloc(sizeof(char) * 4);
	is_malloc_failed(letters);
	letters[0] = first;
	letters[1] = second;
	letters[2] = '\'';
	letters[3] = '\0';
	parsed->err_msg = ft_strjoin(msg, letters);
	free(letters);
}

char	*put_all_letters(char first, char second, char third)
{
	char	*letters;

	letters = (char *)malloc(sizeof(char) * 5);
	is_malloc_failed(letters);
	letters[0] = first;
	letters[1] = second;
	letters[2] = third;
	letters[3] = '\'';
	letters[4] = '\0';
	return (letters);
}
