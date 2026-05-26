/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 18:19:04 by viviane           #+#    #+#             */
/*   Updated: 2025/03/08 18:28:38 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*Ces fonctions permettent d'analyser des caracteres specifiques dans
une ligne de commande et de signaler des erreurs de syntaxes selon le besoin.
Chacune de ces fonctions initialisent (= '\0')trois variable, first(i),
second(i+ 1), third (i + 2), qui stockent les caracteres specifiques a analyser.
Ces trois char sont passes a syntax_err_msg qui renvoie le message d'erreur
correspondant ou la traitement a adopter*/
/*Cette fonction traite le caractere | dans la ligne de commande.
Elle verifie si le caractere suivant est & ou | (|& ou ||).
Et elle traite le caractere via syntax_error_msg*/
void	before_pipe(t_parsing *parsed, char *line, int i)
{
	char	first;
	char	second;
	char	third;

	first = '\0';
	second = '\0';
	third = '\0';
	first = line[i];
	if (line[i + 1] == '&' || line[i + 1] == '|')
		second = line[i + 1];
	syntax_err_msg(parsed, first, second, third);
}

/*Cette fonction traite le caractere de redirection d'entree (<) dans la ligne
de commande.
Elle verifie si le caractere suivant est un &, > ou < (<&, <>, <<).
Elle verifie egalement le cas de <<<.
Et elle traite cette ligne via syntax_err_msg*/
void	before_entry(t_parsing *parsed, char *line, int i)
{
	char	first;
	char	second;
	char	third;

	first = '\0';
	second = '\0';
	third = '\0';
	first = line[i];
	if (line[i + 1] == '&' || line[i + 1] == '>' || line[i + 1] == '<')
		second = line[i + 1];
	if (line[i + 1] == '<' && line[i + 2] == '<')
		third = line[i + 2];
	syntax_err_msg(parsed, first, second, third);
}

/*Cette fonction traite le caractere de redirection de sortie (>) mal utilisées,
dans la ligne de commande, après un heredoc.
Elle verifie si le caractere suivant est un &, > ou < (>&, >| ou >>)*/
void	before_exit(t_parsing *parsed, char *line, int i)
{
	char	first;
	char	second;
	char	third;

	first = '\0';
	second = '\0';
	third = '\0';
	first = line[i];
	if (line[i + 1] == '&' || line[i + 1] == '|' || line[i + 1] == '>')
		second = line[i + 1];
	syntax_err_msg(parsed, first, second, third);
}

/*Cette fonction traite le caractere & dans la ligne de commande
Elle verifie si le caractereb suivant est un & (&&).*/
void	before_esper(t_parsing *parsed, char *line, int i)
{
	char	first;
	char	second;
	char	third;

	first = '\0';
	second = '\0';
	third = '\0';
	first = line[i];
	if (line[i + 1] == '&')
		second = line[i + 1];
	syntax_err_msg(parsed, first, second, third);
}
