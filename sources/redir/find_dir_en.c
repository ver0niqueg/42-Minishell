/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_dir_en.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viviane <vihane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 18:13:34 by viviane           #+#    #+#             */
/*   Updated: 2025/03/16 18:13:35 by viviane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	put_cursors(char *exp, int end, int *first, int *last)
{
	*first = end - 1;
	while (*first - 1 >= 0 && exp[*first] != '/')
		(*first)--;
	*last = 0;
}

void	next_dir(char *exp, int *last, int end)
{
	while (exp[*last] == '/')
		(*last)++;
	if (exp[*last])
		(*last)++;
	while (*last <= end && exp[*last] != '/' )
		(*last)++;
}