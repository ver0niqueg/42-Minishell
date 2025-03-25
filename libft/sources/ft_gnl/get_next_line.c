/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihane <vihane@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 19:10:14 by vihane            #+#    #+#             */
/*   Updated: 2025/01/17 20:23:03 by vihane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/get_next_line.h"

char	*extract_real_line(char *remaining_buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (!remaining_buffer)
		return (NULL);
	while (remaining_buffer[i] && remaining_buffer[i] != '\n')
		i++;
	if (remaining_buffer[i] == '\n')
		i++;
	line = get_substr(remaining_buffer, 0, i);
	if (!line)
		return (NULL);
	line[i] = '\0';
	return (line);
}

char	*extract_the_line(char *line, char *real_line)
{
	char	*remaining_buffer;
	int		i;

	i = 0;
	if (!line || !real_line)
		return (free(real_line), NULL);
	while (line[i] && line[i] != '\n')
		i++;
	remaining_buffer = get_substr(line, i + 1, get_strlen(line) - (i + 1));
	if (!remaining_buffer)
		return (NULL);
	if (*remaining_buffer == '\0')
	{
		free(remaining_buffer);
		return (NULL);
	}
	return (remaining_buffer);
}

char	*set_the_buffer_line(int fd, char *remaining_buffer, char *buffer)
{
	char	*char_tmp;
	int		read_line;

	read_line = 1;
	while (read_line != 0)
	{
		read_line = read(fd, buffer, BUFFER_SIZE);
		if (read_line == -1)
			return (0);
		else if (read_line == 0)
			break ;
		buffer[read_line] = '\0';
		if (!remaining_buffer)
			remaining_buffer = get_strdup("");
		if (!remaining_buffer)
			return (NULL);
		char_tmp = remaining_buffer;
		remaining_buffer = get_strjoin(char_tmp, buffer);
		free(char_tmp);
		if (!remaining_buffer)
			return (NULL);
		if (get_strchr(buffer, '\n'))
			break ;
	}
	return (remaining_buffer);
}

char	*get_initialisation_buffer(int fd, char **remaining_buffer)
{
	char	*buffer;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer || read(fd, 0, 0) < 0 || fd < 0 || BUFFER_SIZE <= 0
		|| fd > MAX_FD)
	{
		if (*remaining_buffer)
		{
			free(*remaining_buffer);
			*remaining_buffer = NULL;
		}
		free(buffer);
		return (NULL);
	}
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*remaining_buffer[MAX_FD];
	char		*buffer_line;
	char		*line;
	char		*real_line;

	buffer_line = get_initialisation_buffer(fd, &remaining_buffer[fd]);
	if (!buffer_line)
		return (NULL);
	line = set_the_buffer_line(fd, remaining_buffer[fd], buffer_line);
	free(buffer_line);
	if (!line)
		return (NULL);
	real_line = extract_real_line(line);
	if (!real_line)
		return (free(line), NULL);
	remaining_buffer[fd] = extract_the_line(line, real_line);
	free(line);
	return (real_line);
}
