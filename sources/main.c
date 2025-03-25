/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 12:17:51 by vihane           #+#    #+#             */
/*   Updated: 2025/03/08 18:00:40 by vihane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_split(char **strs)
{
	int	i;

	i = -1;
	while (strs[++i] != NULL)
		free(strs[i]);
	free(strs);
}

static char	**duplicate_envp(char **envp)
{
	char	**dup_envp;
	int		count;
	int		i;

	count = 0;
	while (envp[count] != NULL)
		count++;
	dup_envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (dup_envp == NULL)
		return (NULL);
	i = 0;
	while (i < count)
	{
		dup_envp[i] = ft_strdup(envp[i]);
		is_failed_malloc(dup_envp[i]);
		i++;
	}
	dup_envp[count] = NULL;
	return (dup_envp);
}

void prompt(t_minishell *minishell)
{
    char *prompt;

    while (true)
    {
        prompt = readline("Cuties$ ");
        if (prompt == NULL)
            return ;
        else if (prompt[0] == '\0' || ft_empty_line(prompt))
        {
            free(prompt);
            continue ;
        }
        else if (ft_strlen(prompt) > 0)
        {
            understand_line(prompt, minishell);
            if (minishell->parsed[0].err_nb == 2)
                print_error(minishell->parsed[0].err_msg);
            (add_history(prompt), free(prompt));
            if (0 != request_limiters(minishell->parsed, minishell->nb_of_cmds))
                continue ;
            free_parsed(minishell->parsed, minishell->nb_of_cmds);
        }
    }
}

int main(int argc, char **argv, char **envp)
{
    t_minishell minishell;

    (void)argv;

    if (argc != 1)
        printf("Error, no arguments needed\n"), exit(EXIT_FAILURE);
    minishell.envp = duplicate_envp(envp);
    if (minishell.envp == NULL)
        return (EXIT_FAILURE);
    minishell.ex_nb = 0;
    prompt(&minishell);
    free_split(minishell.envp);
    return (EXIT_SUCCESS);
}