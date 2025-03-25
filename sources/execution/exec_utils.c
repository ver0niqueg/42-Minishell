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

// ** pour manipuler des tableaux de chaines de caracteres

/* fonction qui cherche PATH et extrait sa valeur -> utile pour executer une
cmd en recherchant son executable dans ces repertoires */
char    **find_env_paths(char **env)
{
    int     i; // compteur pour parcourir le tableau env (des variables d'env)
    char    **paths; // tableau des chemins extraits de PATH

    i = -1;
    // recherche de la variable PATH dans env
    while (env[++i] != NULL)
    {
        if (ft_strncmp(env[i], "PATH=", 5) == 0)
            break;
    }
    if (env[i] == NULL)
        return (NULL);
    // extraction de la valeur de PATH
    paths = ft_split(env[i] + 5, ':'); // PATH=/usr/local/bin:/usr/bin:/bin:/home/user/bin
    if (paths == NULL) // check de l'allocation de memoire
        return (NULL);
    return (paths);
}

/* fonction qui parcourt les repertoires du PATH pour trouver un executable
correspondant a la cmd donnee et met a jour args[0] avec son chemin complet
si trouve */
char    **matching_path(char **args, char **env_paths)
{
    int     i; // pour parcourir env_paths
    char    *cmd; // stocke une version modifiee de la cm avec / ajoute devant
    char    *cpy; // une copie de args[0] pour le restaurer si aucun chemin n'est trouve

    i = -1;
    cmd = ft_strjoin("/", args[0]); // ex: "/ls"
    is_malloc_failed(cmd);
    cpy = ft_strdup(args[0]);
    is_malloc_failed(cpy);
    // boucle de recherche du chemin correct
    while (env_paths[++i] != NULL)
    {
        free(args[0]); // libere l'ancienne valeur
        args[0] = ft_strjoin(env_paths[i], cmd); // concatene un chemin du PATH + cmd
        is_malloc_failed(args[0]);
        // check si le fichier est executable
        if (access(args[0], X_OK) == 0)
        {
            (free(cmd), free(cpy), free_split(env_paths));
            return (args); // retourne args avec le chemin complet de la cmd
        }
    }
    // si aucun chemin n'est valide
    if (args[0])
        free(args[0]);
    args[0] = cpy;
    (free_split(env_paths), free(cmd));
    return (args);
}

/* fonction qui transsforme une cmd simple en chemin executable */
char    **is_executable(char **args, char **env_paths, char **env)
{
    if (ft_strchr(args[0], '/') == NULL) // check si args[0] contient un /
    // si args[0] ne contient pas de / -> signifie que c'est une cmd simple (ls, grep) et non un chemin (ex : "/bin/ls")
    {
        // recup des chemins du PATH si necessaire
        env_paths = find_env_paths(env);
        if (env_paths == NULL)
            return (args);
        // recherche du chemin complet de la cmd
        args = matching_path(args, env_paths);
    }
    return (args);
}