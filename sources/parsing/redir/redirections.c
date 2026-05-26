/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:08:51 by viviane           #+#    #+#             */
/*   Updated: 2025/03/12 21:08:52 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*Permet de traiter les redirections de type heredoc (<<), elle squte tout
d'abord le deuxième <, et ignore les espaces si elle en trouve.
Elle extrait le délimiteur. et le socke dans la structure. */
int	handle_heredoc(char *line, int start, t_parsing *parsed, int *limiter_size)
{
	int		end;
	char	*limiter;

	limiter = NULL;
	start++;
	ft_skip_spaces(&start, line);
	end = start;
	while (find_end_of_word(&end, line) == 1)
		ft_substring(line + start, end - start - 1, &limiter);
	if (limiter != NULL)
	{
		add_str(&parsed->limiter, limiter, limiter_size);
		if (parsed->entry != NULL)
		{
			free(parsed->entry);
			parsed->entry = NULL;
		}
		free(limiter);
	}
	return (end);
}

/*Permet de gérer les redirection simple. On nettoie l'ancienne
entrée si elle existe, on extrait le nom du fichier.
On vérifie si il y a des erreurs (si le fichier existe), et si il
y a des variables à étendre*/
int	handle_entry(char *line, int start, t_parsing *parsed,
		t_minishell *minishell)
{
	int	end;

	end = start;
	if (parsed->entry != NULL)
	{
		free(parsed->entry);
		parsed->entry = NULL;
	}
	while (find_end_of_word(&end, line) == 1)
		ft_substring(line + start, end - start - 1, &parsed->entry);
	if (parsed->entry != NULL)
	{
		entry_error(parsed->entry, parsed, minishell);
		expand(&(parsed->entry), minishell);
	}
	return (end);
}

/*Permet de gérer les redirections de sortie (>, écrase le fichier,
>>, ajoute à la fin du fichier). Par défaut on set ex_append à 0 ce qui équivaut
au mode écrasement et à 1 pour le mode ajout.
alloc_or_free permet de gérer les erreurs et expansion et les permissions.*/
int	handle_exit(char *line, int start, t_parsing *parsed,
		t_minishell *minishell)
{
	int	end;
	int	on_sign;

	on_sign = start - 1;
	parsed->ex_append = 0;
	if (line[start] == '>')
	{
		start++;
		ft_skip_spaces(&start, line);
		parsed->ex_append = 1;
	}
	ft_skip_spaces(&start, line);
	end = start;
	if (parsed->exit != NULL)
	{
		free(parsed->exit);
		parsed->exit = NULL;
	}
	while (find_end_of_word(&end, line) == 1)
		ft_substring(line + start, end - start - 1, &parsed->exit);
	alloc_or_free(on_sign, line, parsed, minishell);
	return (end);
}
