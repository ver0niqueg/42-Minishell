/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:16:06 by viviane           #+#    #+#             */
/*   Updated: 2025/03/12 21:16:07 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void    entry_error(char *entry, t_parsing *parsed, t_minishell *minishell)
{
    	char	*exp;

	exp = NULL;
	define_exp(&exp, entry, minishell);
	if (entry[0] == '$'
		&& !ft_strchr(entry, '\'') && !ft_strchr(entry, '"'))
		if (amb_redir(entry, exp, parsed))
			return ;
	exp = erase_quote(exp);
	if (not_readable(&exp, parsed))
		return ;
	if (count_chars(exp, '/') > 0)
		if (path_error_en(exp, parsed))
			return ;
	if (access(exp, F_OK) == -1)
		return (found_error(parsed, exp, NULL,
				": No such file or directory"));
	return (free(exp));
}

void    exit_error(char *exit, t_parsing *parsed, t_minishell *minishell)
{
	char		*exp;
	struct stat	sb;

	define_exp(&exp, exit, minishell);
	if (exit[0] == '$' && !ft_strchr(exit, '\'') && !ft_strchr(exit, '"'))
		if (amb_redir(exit, exp, parsed))
			return ;
	exp = erase_quote(exp);
	if (!exp || always_a_dir(exp, ft_strlen(exp) - 1, parsed))
		return ;
	if (access(exp, W_OK) == 0 && stat(exp, &sb) == 0)
		if (bad_redir_gd_file(exp, parsed, sb))
			return ;
	if (count_chars(exp, '/') > 0)
		if (path_error_ex((int)ft_strlen(exp) - 1, exp, parsed))
			return ;
	if (access(exp, W_OK) != 0 && access(exp, F_OK) == 0)
		return (found_error(parsed, exp, NULL, ": Permission denied"));
	if (!exp[0])
		return (found_error(parsed, exp, NULL, ": No such file or directory"));
	open_file(exp, parsed);
	return (free(exp));
}