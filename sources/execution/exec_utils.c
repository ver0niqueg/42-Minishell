/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:04:53 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/11 14:04:53 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* fonction qui cherche PATH et extrait sa valeur -> utile pour executer une
cmd en recherchant son executable dans ces repertoires */
char	**find_env_paths(char **env)
{
	int		i;
	char	**paths;

	i = -1;
	while (env[++i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			break ;
	}
	if (env[i] == NULL)
		return (NULL);
	paths = ft_split(env[i] + 5, ':');
	if (paths == NULL)
		return (NULL);
	return (paths);
}

/* fonction qui parcourt les repertoires du PATH pour trouver un executable
correspondant a la cmd donnee et met a jour args[0] avec son chemin complet
si trouve */
char	**matching_path(char **args, char **env_paths)
{
	int		i;
	char	*cmd;
	char	*cpy;

	i = -1;
	cmd = ft_strjoin("/", args[0]);
	is_malloc_failed(cmd);
	cpy = ft_strdup(args[0]);
	is_malloc_failed(cpy);
	while (env_paths[++i] != NULL)
	{
		free(args[0]);
		args[0] = ft_strjoin(env_paths[i], cmd);
		is_malloc_failed(args[0]);
		if (access(args[0], X_OK) == 0)
		{
			(free(cmd), free(cpy), free_split(env_paths));
			return (args);
		}
	}
	if (args[0])
		free(args[0]);
	args[0] = cpy;
	(free_split(env_paths), free(cmd));
	return (args);
}

/* fonction qui transforme une cmd simple en chemin executable */
char	**is_executable(char **args, char **env_paths, char **env)
{
	if (ft_strchr(args[0], '/') == NULL)
	{
		env_paths = find_env_paths(env);
		if (env_paths == NULL)
			return (args);
		args = matching_path(args, env_paths);
	}
	return (args);
}
