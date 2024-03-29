/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmehlig <jmehlig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:45:11 by jmehlig           #+#    #+#             */
/*   Updated: 2021/08/25 14:53:23 by jmehlig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

//actually writing the rightly cut line to a variable
static char	*ft_new_line(char *readbuffer)
{
	char	*new_line;
	int		i;

	i = 0;
	while (readbuffer[i] != '\n' && readbuffer[i] != '\0')
		i++;
	new_line = malloc(i + 1);
	if (!new_line)
		return (NULL);
	i = 0;
	while (readbuffer[i] != '\n' && readbuffer[i] != '\0')
	{
		new_line[i] = readbuffer[i];
		i++;
	}
	new_line[i] = '\0';
	return (new_line);
}

//sets the pointer of the static variable to the new line
static char	*rearrenge_buffer(char *readbuffer, int len)
{
	char	*new_readbuffer;
	char	*temp;

	new_readbuffer = malloc(ft_strlen(&readbuffer[len + 1]) + 1);
	temp = new_readbuffer;
	if (!new_readbuffer)
		return (NULL);
	while (readbuffer[len + 1] != '\0')
	{
		*temp = readbuffer[len + 1];
		temp++;
		len++;
	}
	*temp = '\0';
	free(readbuffer);
	return (new_readbuffer);
}

//the static variable readbuffer is the pointer inside the file
//for the first call of the function it has to be allocated
//gives the read line to the buffer, if '\n' is the case it cuts everything from the new line
//and rearranges the buffer
static char	*print_line(char *line)
{
	static char	*readbuffer;
	char		*result;

	if (!readbuffer)
	{
		readbuffer = malloc(1);
		*readbuffer = '\0';
	}
	if (!readbuffer)
		return (NULL);
	if (ft_strlen(line) > 0)
		readbuffer = ft_strjoin(readbuffer, line);
	free(line);
	if (ft_strlen(readbuffer) == 0)
		return (get_result(&readbuffer, (char *) NULL));
	result = ft_new_line(readbuffer);
	if (readbuffer[ft_strlen(result)] == '\n')
	{
		readbuffer = rearrenge_buffer(readbuffer, ft_strlen(result));
		return (ft_strjoin(result, "\n"));
	}
	else
		return (get_result(&readbuffer, result));
}

//reads from the file with descriptor fd till '\n' or EOF are reached
//the complete line gets stored in line
char	*read_next_line(int fd, char *line, char *buffer, int bytesread)
{
	while (bytesread > 0)
	{
		buffer[bytesread] = '\0';
		if (!ft_strchr(buffer, '\n'))
		{
			line = ft_strjoin(line, buffer);
			bytesread = read(fd, buffer, BUFFER_SIZE);
		}
		else
		{
			line = ft_strjoin(line, buffer);
			free(buffer);
			return (print_line(line));
		}
	}
	free(buffer);
	return (print_line(line));
}

// reads one line of a file, stops if '\n' or EOF is reached
char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	int			bytesread;

	buffer = malloc(BUFFER_SIZE + 1);
	line = malloc(1);
	*line = '\0';
	bytesread = read(fd, buffer, BUFFER_SIZE);
	return (read_next_line(fd, line, buffer, bytesread));
}
