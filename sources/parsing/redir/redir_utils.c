/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 23:59:09 by viviane           #+#    #+#             */
/*   Updated: 2025/03/13 23:59:10 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	open_file(char *exp, t_parsing *parsed)
{
	int		fd;

	if (parsed->ex_append == 1)
		fd = open(exp, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(exp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		(print_error("Open error"), exit(EXIT_FAILURE));
	close(fd);
}

void    alloc_or_free(int on_sign, char *line, t_parsing *parsed, 
                        t_minishell *minishell)
{
    if (parsed->exit && parsed->exit[0] != '\0')
    {
        exit_error(parsed->exit, parsed, minishell);
        if (on_sign - 1 >= 0 && line[on_sign - 1] == '<')
        {
            free(parsed->exit);
            parsed->exit = NULL;
        }
        else
            expand(&(parsed->exit), minishell);
    }
}

void   define_exp(char **dest, char *src, t_minishell *minishell)
{
    (*dest) = ft_strdup(src);
    is_malloc_failed(*dest);
    expand(dest, minishell);
}

void    found_error(t_parsing *parsed, char *exp, 
                    char *dir_or_fil, const char *msg)
{
    parsed->err_msg = ft_strjoin(msg, exp);
    is_malloc_failed(parsed->err_msg);
    parsed->err_nb = 1;
    free_redir(parsed);
    if (exp)
        free(exp);
    if (dir_or_fil)
        free(dir_or_fil);
}

int last_word(int first, int last)
{
    if (last > first)
        return (1);
    return (0); 
}