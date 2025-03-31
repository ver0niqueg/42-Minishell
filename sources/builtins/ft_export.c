/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:42:46 by vgalmich          #+#    #+#             */
/*   Updated: 2025/03/12 22:42:46 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	export_write_error(const char *s1, const char *s2, const char *s3)
{
	write(STDERR_FILENO, s1, ft_strlen(s1));
	write(STDERR_FILENO, "'", 1);
	write(STDERR_FILENO, s2, ft_strlen(s2));
	write(STDERR_FILENO, "'", 1);
	write(STDERR_FILENO, s3, ft_strlen(s3));
	write(STDERR_FILENO, "\n", 1);
}

/* fonction pour trouver la key d'une variable d'env */
bool	is_env_key(char *env_var)
{
	int	i;

	i = 0;
	if (ft_isdigit(env_var[0]) || (ft_isalpha(env_var[0]) == 0
			&& env_var[0] != '_'))
		return (false);
	while (env_var[i] != '\0')
	{
		if (ft_isalnum(env_var[i]) == 0 && env_var[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

/* fonction qui sert a afficher les variables d'env sous un
format specifique utilise par la cmd export */
static int	print_export_envp(char **envp, t_fd output_fd)
{
	int	key_len;
	int	value_len;
	int	return_code;

	return_code = 0;
	while (*envp)
	{
		key_len = ft_strchr(*envp, '=') - *envp;
		value_len = *envp + ft_strlen(*envp) - (ft_strchr(*envp, '=') + 1);
		// affichage de la variable d'env dans le format declare -x
		write(output_fd, "declare -x", 11);
		write(output_fd, *envp, key_len);
		write(output_fd, "=\"", 2);
		write(output_fd, *envp + key_len + 1, value_len);
		write(output_fd, "\"\n", 2);
		if (errno != 0)
		{
			write_error_msg("minishell: export: write error: ",
				strerror(errno));
			return_code = 1;
			break ;
		}
		envp++;
	}
	return (return_code);
}

/* fonction qui ajoute une nouvelle variable d'env ou met a jour une 
variable d'env eistante dans un tableau d'env */
int	add_env_var(char **args, char ***envp)
{
	int	return_code;

	return_code = 0;
	// separation de la cle de la valeur args = KEY=VALUE
	(*args)[ft_strchr(*args, '=') - *args] = '\0';
	// check si la clef est valide
	if (is_env_key(*args) == true)
	{
		// maj ou ajout de variable d'env
		if (ft_set_env(envp, *args,
				*args + ft_strlen(*args) + 1, 1) == -1)
		{
			// gestion des erreurs
			(*args)[ft_strlen(*args)] = '=';
			return (write_error_msg("minishell: export: ", strerror(errno)), 1);
		}
	}
	else // cas ou la clef n'est pas valide
		(export_write_error("minishell: export: ",
				*args, ": not a valid identifier"), return_code = 1);
	(*args)[ft_strlen(*args)] = '=';
	return (return_code);
}

/* fonction qui ajoute des variables d'env ou les affiche si aucun
argument n'est passe */
int	ft_export(char **args, t_fd input_fd, t_fd output_fd, char ***envp)
{
	int	return_code;

	(void)input_fd;
	return_code = 0;
	if (*args == NULL)
		return (print_export_envp(*envp, output_fd));
	while (*args != NULL)
	{
		if (NULL != ft_strchr(*args, '=') && (*args)[0] != '=')
			return_code = add_env_var(args, envp);
		else if (is_env_key(*args) == false)
		{
			export_write_error("minishell: export: ",
				*args, ": not a valid identifier");
			return_code = 1;
		}
		args++;
	}
	return (return_code);
}
