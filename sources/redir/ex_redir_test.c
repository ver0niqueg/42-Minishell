/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_test.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:57:04 by viviane           #+#    #+#             */
/*   Updated: 2025/03/15 14:57:05 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	amb_redir(char *src, char *expanded, t_parsing *parsed)
{
	char	*buffer;

	if (expanded == NULL)
	{
		buffer = ft_strjoin(src, ": Ambiguous redirect");
		parsed->err_msg = ft_strdup(buffer);
		free(buffer);
		parsed->err_nb = 1;
		free_redir(parsed);
		return (1);
	}
	return (0);
}

void	path_without_pts(char **no_points, char *exp, int end)
{
	*no_points = ft_strndup(exp, end);
	if ((*no_points) == NULL)
		*no_points = ft_strdup(exp);
	is_failed_malloc((*no_points));
}

int	always_a_dir(char *exp, int end, t_parsing *parsed)
{
	int		points;
	char	*no_points;

	points = 0;
	no_points = NULL;
	while (exp[points] && exp[points] == '.')
		points++;
	if (exp[0] && points <= 2 && exp[end] == '/')
	{
		while (exp[points] && exp[points] == '/')
			points++;
		while ((end - 1 >= 0) && exp[end] == '/')
			end--;
		path_without_pts(&no_points, exp + points, end - points);
		if (count_chars(no_points, '.') == 0
			&& count_chars(no_points, '/') == 0)
		{
			parsed->err_msg = ft_strjoin(exp, ": Is a directory");
			(is_failed_malloc(no_points), parsed->err_nb = 1);
			(free_redir(parsed), free(no_points), free(exp));
			return (1);
		}
		free(no_points);
	}
	return (0);
}

int	bad_redir_gd_file(char *exp, t_parsing *parsed, struct stat sb)
{
	int	fd;

	if (S_ISDIR(sb.st_mode))
	{
		parsed->err_msg = ft_strjoin(exp, ": Is a directory");
		parsed->err_nb = 1;
		(free_redir(parsed), free(exp));
		return (1);
	}
	if (S_ISREG(sb.st_mode) || S_ISCHR(sb.st_mode))
	{
		if (parsed->ex_append == 1)
			fd = open(exp, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(exp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			(print_error("Open error"), exit(EXIT_FAILURE));
		(close(fd), free(exp));
		return (1);
	}
	return (0);
}

int	path_error_ex(int end_of_path, char *exp, t_parsing *parsed)
{
	t_check_path	cp;

	check_path_init(&end_of_path, exp, &cp);
	while (exp[cp.last] && cp.last <= end_of_path)
	{
		end_of_dir(exp, &cp.last);
		if (exp[cp.last] && cp.last <= end_of_path)
		{
			cp.dir_or_fil = ft_substr(exp, 0, cp.last);
			is_failed_malloc(cp.dir_or_fil);
			if (access(cp.dir_or_fil, W_OK) == 0
				&& stat(cp.dir_or_fil, &cp.sb) == 0)
			{
				if (wrong_data_type(&cp, parsed, exp, end_of_path))
					return (1);
			}
			else
				if (not_writable(&cp, parsed, exp, end_of_path))
					return (1);
			free(cp.dir_or_fil);
		}
	}
	return (0);
}

