/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 01:16:22 by vgalmich          #+#    #+#             */
/*   Updated: 2025/02/21 01:16:22 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* fonction qui recherche et renvoie la valeur d'une variable d'env dans un
tableau env a partir de son nom */
char	*ft_get_env(const char **envp, const char *key)
{
	int		i;
	size_t	key_len;

	if (key == NULL || key[0] == '\0')
		return (NULL);
	i = 0;
	while (envp[i] != NULL)
	{
		key_len = ft_strchr(envp[i], '=') - envp[i];
		if (ft_strncmp(envp[i], key, key_len) == 0
			&& ft_strlen(key) == key_len)
			return (ft_strchr(envp[i], '=') + 1);
		i++;
	}
	return (NULL);
}

/* fonction qui ajoute une nouvelle variable d'env et remplace donc le tableau
avec un nouveau */
static int	add_new_key_value(char ***envp, const char *key, const char *value)
{
	char	**new_envp;
	int		i;
	size_t	keyvalue_len;

	i = 0;
	while ((*envp)[i] != NULL)
		i++;
	new_envp = (char **)malloc(sizeof(char *) * (i + 2));
	if (new_envp == NULL)
		return (-1);
	(ft_memcpy(new_envp, *envp, sizeof(char *) * i), new_envp[i + 1] = NULL);
	keyvalue_len = ft_strlen(key) + 1 + ft_strlen(value);
	new_envp[i] = (char *)malloc(sizeof(char) * (keyvalue_len + 1));
	if (new_envp[i] == NULL)
	{
		free(new_envp);
		return (-1);
	}
	ft_memcpy(new_envp[i], key, sizeof(char) * ft_strlen(key));
	ft_memcpy(new_envp[i] + ft_strlen(key), "=", sizeof(char) * 1);
	ft_memcpy(new_envp[i] + ft_strlen(key) + 1, value,
		sizeof(char) * ft_strlen(value));
	new_envp[i][keyvalue_len] = '\0';
	(free(*envp), *envp = new_envp);
	return (0);
}

/* fonction qui supprime une variable d'env */
int	ft_unset_env(char **envp, const char *key)
{
	int		i;
	size_t	key_len;

	if (key == NULL || key[0] == '\0' || ft_strchr(key, '='))
	{
		errno = EINVAL;
		return (-1);
	}
	i = -1;
	while (envp[++i] != NULL)
	{
		key_len = ft_strchr(envp[i], '=') - envp[i];
		if (ft_strncmp(envp[i], key, key_len) == 0 && ft_strlen(key) == key_len)
			break ;
	}
	if (envp[i] != NULL)
	{
		free(envp[i--]);
		while (envp[++i] != NULL)
			envp[i] = envp[i + 1];
	}
	return (0);
}

/* fonction qui ajoute ou modifie une variable d'env (sa valeur) */
int	ft_set_env(char ***envp, const char *var, const char *value, int overwrite)
{
	int		i;
	size_t	key_len;

	i = -1;
	while ((*envp)[++i] != NULL)
	{
		key_len = ft_strchr((*envp)[i], '=') - (*envp)[i];
		if ((ft_strncmp((*envp)[i], var, key_len) == 0)
			&& ft_strlen(var) == key_len)
			break ;
	}
	if (overwrite != 0)
		ft_unset_env(*envp, var);
	if (overwrite == 1 || (*envp)[i] == NULL)
	{
		if (add_new_key_value(envp, var, value) == -1)
		{
			errno = ENOMEM;
			return (-1);
		}
	}
	return (0);
}

/* fonction qui ecrit un message d'erreur sur la sortie standard stderr */
void	write_error_msg(const char *title, const char *error_msg)
{
	write(STDERR_FILENO, title, ft_strlen(title));
	write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
	write(STDERR_FILENO, "\n", 1);
}
