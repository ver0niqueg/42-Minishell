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

/* fonction qui duplique l'envp du processus pour eviter de modifier
l'original */
static char	**copy_envp(char **envp)
{
	char	**cpy_envp;
	int		count;
	int		i;

	count = 0;
	while (envp[count] != NULL)
		count++;
	cpy_envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (cpy_envp == NULL)
		return (NULL);
	i = 0;
	while (i < count)
	{
		cpy_envp[i] = ft_strdup(envp[i]);
		is_malloc_failed(cpy_envp[i]);
		i++;
	}
	cpy_envp[count] = NULL;
	return (cpy_envp);
}

/* fonction qui gere le signal SIGINT (Ctrl+C) -> quitte proprement 
l'interruption du terminal */
void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	//rl_replace_line("", 0);
	rl_redisplay();
}

void write_prompt(t_minishell *minishell)
{
    char *prompt;

    while (true)
    {
        prompt = readline("Cuties$ ");
        if (!prompt)
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
            if (0 != handle_limiters(minishell->parsed, minishell->nb_of_cmds))
                continue ;
            launch_processes(minishell, minishell->nb_of_cmds - 1);
            free_parsed(minishell->parsed, minishell->nb_of_cmds);
        }
    }
    //rl_clear_history();
}

int main(int argc, char **argv, char **envp)
{
    t_minishell minishell;

    (void)argv;
	struct sigaction sa;
	struct sigaction saq;

    if (argc != 1)
        printf("Error, no arguments needed\n"), exit(EXIT_FAILURE);
    	// 1- gestion du signal Ctrl+C (SIGINT) -> empeche de quitter le shell
	sa.sa_handler = &handle_sigint; // definit handle_sigint comme fonction de gestion du signal SIGINT (Ctrl+C)
	sa.sa_flags = SA_RESTART; // redemare les appels systeme interrompus au lieu de les stopper
	(sigemptyset(&sa.sa_mask), sigaddset(&sa.sa_mask, CNTRL_C));
	sigaction(CNTRL_C, &sa, NULL); // active le gestionnaire de signal
	// 2- ignorer Ctrl+\ (SIGQUIT) -> pour eviter un crash
	saq.sa_handler = SIG_IGN;
	saq.sa_flags = SA_RESTART;
	(sigemptyset(&saq.sa_mask), sigaddset(&saq.sa_mask, CNTRL_B_SLASH));
	// 3- duplication de l'env -> pour une gestion propre
	minishell.envp = copy_envp(envp);
	if (minishell.envp == NULL)
		return (EXIT_FAILURE);
	minishell.exit_code = 0;
	// 4- lance l'invite prompt -> pour executer les cmds
    write_prompt(&minishell);
    free_split(minishell.envp);
    return (EXIT_SUCCESS);
}