/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 00:48:51 by vgalmich          #+#    #+#             */
/*   Updated: 2025/02/21 00:48:51 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// la commande cd (change directory) sert a naviguer entre les repertoires
// cd nom_du_dossier -> se deplacer dans un sous dossier du repertoire actuel
// cd .. -> revenir au dossier parent
// cd / -> aller a la racine du systeme de fichiers
// cd ~ -> aller dans le dossier personnel de l'utilisateur (home)
// cd - -> revenir au dernier repertoire visite

/* fonction qui affiche un message d'erreur */
static void cd_error_msg(const char *s1, const char *s2, const char *s3)
{
	write(STDERR_FILENO, s1, ft_strlen(s1));
	write(STDERR_FILENO, s2, ft_strlen(s2));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, s3, ft_strlen(s3));
	write(STDERR_FILENO, "\n", 1);
}

/* fonction qui change de repo vers home */
static int  go_to_home_directory(const char **envp)
{
	char    *home;

	home = ft_get_env(envp, "HOME");
	if (home == NULL)
	{
		write(STDERR_FILENO, "minishell: cd: HOME is undefined\n", 33);
		return (1);
	}
	if (chdir(home) == -1) // on utilise chdir pour changer le repo
	{
		cd_error_msg("minishell: cd: ", strerror(errno), home); // strerror(erno) dafuq??
		return (1);
	}
	return (0);
}

/* fonction qui change de repo */
static int  change_directory(const char *path)
{
	if (-1 == chdir(path))
	{
		cd_error_msg("minishell: cd: ", strerror(errno), path);
		return (1);
	}
	return (0);
}

/* fonction pour maj la variable d'env PWD avec le repo de travail actuel */
static int  update_pwd(char ***envp)
{
	char    pwd[PATH_MAX];

	if (ft_get_env(*(const char ***)envp, "PWD") != NULL)
	{
		if (getcwd(pwd, sizeof(pwd)) == NULL) // getcwd recupere le repo actuel
		{
			write_error_msg("minishell: cd: ", strerror(errno));
			return (1);
		}
		if (-1 == ft_set_env(envp, "PWD", pwd, 1)) // coder ft_setenv
		{
			write_error_msg("minishell: cd: ", strerror(errno));
			return (1);
		}
	}
	return (0);
}

/* fonction qui gere la commande cd */
int ft_cd(char **args, t_fd input_fd, t_fd output_fd, char ***envp)
{
	int nb_args;
	int result;

	(void)input_fd;
	(void)output_fd;
	nb_args = 0;
	result = 0;
	while (args[nb_args] != NULL)
		nb_args++;
	if (nb_args == 0)
		result = go_to_home_directory(*(const char ***)envp);
	else if (nb_args == 1)
		result = change_directory(*args);
	else
	{
		write(STDERR_FILENO, "minishell: cd: too many arguments\n", 34);
		return (1);
	}
	if (result != 0)
		return (result);
	return(update_pwd(envp));
}
