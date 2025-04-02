/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihane <vihane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:16:55 by viviane           #+#    #+#             */
/*   Updated: 2025/04/02 17:58:24 by vihane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*put_all(char first, char second, char third)
{
	char	*letter;

	letter = (char *)malloc(sizeof(char) * 5);
	is_malloc_failed(letter);
	letter[0] = first;
	letter[1] = second;
	letter[2] = third;
	letter[3] = '\'';
	letter[4] = '\0';
	return (letter);
}

void	put_first_and_second(char first, char second, char *err_msg,
		t_parsing *parsed)
{
	char	*letter;

	letter = (char *)malloc(sizeof(char) * 4);
	is_malloc_failed(letter);
	letter[0] = first;
	letter[1] = second;
	letter[2] = '\'';
	letter[3] = '\0';
	parsed->err_msg = ft_strjoin(err_msg, letter);
	free(letter);
}

void	put_first(char first, char *err_msg, t_parsing *parsed)
{
	char	*letter;

	letter = (char *)malloc(sizeof(char) * 3);
	is_malloc_failed(letter);
	letter[0] = first;
	letter[1] = '\'';
	letter[2] = '\0';
	parsed->err_msg = ft_strjoin(err_msg, letter);
	free(letter);
}

/*Fonction pour construire un message d'erreur détaillé en fonction des
caractères fautif, et stocke le message dans parsed->err_msg et définit le
le code d'erreur (parsed->err_nb) à 2.*/
void	syntax_err_msg(t_parsing *parsed, char first, char second, char third)
{
	char	*msg;
	char	*letter;

	msg = ft_strdup("syntax error near unexpected token `");
	is_malloc_failed(msg);
	if (first == '\0')
	{
		parsed->err_msg = ft_strjoin(msg, "newline\'");
		is_malloc_failed(msg);
	}
	else if (first != '\0' && second == '\0')
		put_first(first, msg, parsed);
	else if (first != '\0' && second != '\0' && third == '\0')
		put_first_and_second(first, second, msg, parsed);
	else if (first != '\0' && second != '\0' && third != '\0')
	{
		letter = put_all(first, second, third);
		parsed->err_msg = ft_strjoin(msg, letter);
		free(letter);
	}
	free(msg);
	parsed->err_nb = 2;
}
