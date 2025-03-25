/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:32:22 by vgalmich          #+#    #+#             */
/*   Updated: 2025/02/26 17:32:22 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* fonction qui check si str est vide ou ne contient que des espaces/tabs */
int ft_is_empty(char *str)
{
	int i;
	int	counter;

	i = -1;
	counter = 0;
	if (str == NULL)
		return (1);
	while (str[++i] != '\0')
		if (!(str[i] >= 9 && str[i] <= 13) && str[i] != ' ')
			counter++;
	if (counter == 0)
		return (1);
	return (0);
}

/* fonction qui affiche un prompt hackeuse$ en boucle, lit l'entree 
utilisateur et execute la cmd correspondante */
void write_prompt(t_minishell *minishell)
{
	char *line;

	while (true)
	{
		line = readline("hackeuses$ ");
		if (!line)
			return ;
		else if (line[0] == '\0' ||ft_is_empty(line))
		{
			free(line);
			continue ;
		}
		else if (ft_strlen(line) > 0)
		{
			obtain_cmds(line, minishell);
			if (minishell->parsed[0].error_code == 2)
				print_error(minishell->parsed[0].error_msg);
			(add_history(line), free(line));
			if (handle_limiters(minishell->parsed, minishell->nb_of_cmds != 0))
				continue;
			launch_processes(minishell, minishell->nb_of_cmds - 1);
			free_all_parsed(minishell->parsed, minishell->nb_of_cmds);
		}
	}
	rl_clear_history();
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
		malloc_failure(cpy_envp[i]);
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
	rl_replace_line("", 0);
	rl_redisplay();
}

int main(int argc, char **argv, char **envp)
{
	t_minishell			minishell;
	struct sigaction	sa;
	struct sigaction	saq;

	(void)argv;
	if (argc != 1)
		(printf("No arguments needed\n"), exit(EXIT_FAILURE));
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