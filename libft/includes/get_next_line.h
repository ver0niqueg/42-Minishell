/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgalmich <vgalmich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:47:36 by vihane            #+#    #+#             */
/*   Updated: 2025/04/01 23:59:15 by vgalmich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# ifndef MAX_FD
#  define MAX_FD 1024
# endif

char	*extract_real_line(char *line);
char	*extract_rest_line(char *line);
char	*set_the_buffer_line(int fd, char *tmp, char *line_buffer);
char	*get_next_line(int fd);
char	*get_strjoin(char const *s1, char const *s2);
char	*get_strdup(char const *s);
char	*get_substr(const char *str, unsigned int start, size_t len);
char	*get_strchr(const char *s, int c);
char	*get_initialisation_buffer(int fd, char **remaining_buffer);

size_t	get_strlen(const char *str);

#endif
