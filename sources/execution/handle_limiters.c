/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_limiters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:50:43 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/10 15:50:43 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* fonction qui libere la memoire de heredoc_data pour toutes les cmds */
static void free_limiters(t_parsing *p, int nb_of_cmds)
{
	int i;

	i = 0;
	while (i < nb_of_cmds)
	{
		free(p[i].heredoc_data);
		p[i].heredoc_data = NULL;
		i++;
	}
}

/* fonction qui duplique une chaine et ajoute un saut de ligne \n */
static char	*ft_strdup_nl(const char *str)
{
	size_t	str_len;
	char	*str_nl;

	str_len = ft_strlen(str);
	str_nl = (char *)malloc(sizeof(char) * (str_len + 1));
	if (str_nl == NULL)
		return (NULL);
	ft_memcpy(str_nl, str, str_len);
	ft_memcpy(str_nl + str_len, "\n", sizeof(char) * (1 + 1));
	return (str_nl);
}

/* fonction qui ajoute une ligne de texte a une chaine existante (heredoc_data)
en inserant un saut de ligne apres. Elle est utilisee pour stocker plusieurs
lignes dans un heredoc (<<) */
static char	*append_heredoc_line(const char *heredoc_data, const char *line)
{
	size_t	heredoc_data_len;
	size_t	line_len;
	char	*joined_str;

	if (heredoc_data == NULL)
		return (ft_strdup_nl(line));
	if (line == NULL)
		return (ft_strdup_nl(heredoc_data));
	heredoc_data_len = ft_strlen(heredoc_data);
	line_len = ft_strlen(line);
	joined_str = (char *)malloc(sizeof(char) * (heredoc_data_len + line_len + 2));
	if (joined_str == NULL)
		return (NULL);
	ft_memcpy(joined_str, heredoc_data, sizeof(char) * heredoc_data_len);
	ft_memcpy(joined_str + heredoc_data_len, line, sizeof(char) * line_len);
	ft_memcpy(joined_str + heredoc_data_len + line_len, "\n", sizeof(char) * 2);
	return (joined_str);
}

/* fonction qui lit les entrees de l'utilisateur ligne par ligne jusqu'a ce 
qu'il tape le limiter (mot-clef qui marque la fin) */
static int	read_heredoc(const char *limiter, char **data, 
	bool save_data)
{
	char	*line;
	char	*tmp;

	*data = NULL;
	while (true)
	{
		line = readline("> ");
		if (line == NULL) // erreur ou EOF -> Ctrl+D
			return (-1);
		if (ft_strcmp(limiter, line) == 0)
			break ;
		if (save_data)
		{
			tmp = append_heredoc_line(*data, line);
			if (tmp == NULL)
				return ((free(*data), free(line), -1));
			(free(*data), *data = tmp);
		}
		free(line);
	}
	free(line);
	return (0);
}

/* fonction qui gere les here-doc dans son ensemble
1- collecte les donnees des heredoc pour chaque cmd dans p
2- demande a l'utilisateur d'entrer des lignes jusqu'a ce que le limiter soit rencontre pour chaque cmd
3- gere les erreurs en liberant la memoire si un pb survient pdt la collecte des donnees des limiteurs
4- stocke les lignes collectees dans lim_data pour chaque cmd correspondante */
int	handle_limiters(t_parsing *p, int nb_cmds)
{
	int	i; // pour parourir chaque cmd dans le tableau p
	int	j; // pour parcourir les limiteurs associes a chaque cmd

	i = 0;
	while (i < nb_cmds)
	{
		j = 0;
		while (p[i].limiter != NULL && p[i].limiter[j] != NULL)
		{
			if (0 != read_heredoc(p[i].limiter[j], &p[i].heredoc_data,
					p[i].limiter[j + 1] == NULL)) // appelle la fonction qui attend une entree jusqu'a ce que l'utilisateur tape un limiter egal a p[i].lim[i]
			{
				free_limiters(p, nb_cmds);
				free_all_parsed(p, nb_cmds);
				return (-1);
			}
			j++;
		}
		i++;
	}
	return (0);
}