/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtrendaf <dtrendaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:33:18 by dtrendaf          #+#    #+#             */
/*   Updated: 2024/10/30 16:25:58 by dtrendaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	char		*buffer;
	char		*line;
	char		*new_line;
	static char	*buffer_remainder;

	if (BUFFER_SIZE < 1 || fd < 0 || read(fd, &buffer, 0) < 0)
		return (free(buffer_remainder), buffer_remainder = NULL, NULL);
	new_line = empty_remainder_and_first_read(fd, buffer, &buffer_remainder);
	if (new_line == NULL)
		return (NULL);
	line = read_and_find(fd, buffer, &buffer_remainder, new_line);
	return (line);
}

char	*read_and_find(int fd, char *buffer, char **remainder, char *new_line)
{
	int		bytes_read;
	char	*temp;

	while (ft_strchr(new_line, '\n') == NULL)
	{
		buffer = malloc(BUFFER_SIZE + 1);
		if (buffer == NULL)
			return (free(new_line), NULL);
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			return (free(buffer), remainder = NULL, new_line);
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(new_line, buffer);
		free(buffer);
		free(new_line);
		if (temp == NULL)
			return (NULL);
		new_line = temp;
	}
	temp = divide(new_line, remainder);
	if (temp == NULL)
		return (free(new_line), NULL);
	return (free(new_line), temp);
}

char	*divide(char *new_line, char **remainder)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	j = -1;
	while (new_line[i] != '\n')
		i++;
	temp = malloc(i + 2);
	if (temp == NULL)
		return (NULL);
	while (++j < i)
		temp[j] = new_line[j];
	ft_strlcpy(&temp[j], "\n", 2);
	if (new_line[i + 1] != '\0')
	{
		*remainder = malloc(ft_strlen(new_line) - ft_strlen(temp) + 1);
		if (*remainder == NULL)
			return (free(temp), NULL);
		j = -1;
		while (new_line[i] != '\0')
			(*remainder)[++j] = new_line[++i];
		(*remainder)[j] = '\0';
	}
	return (temp);
}

char	*empty_remainder_and_first_read(int fd, char *buffer, char **remainder)
{
	int		bytes_read;
	char	*temp;

	temp = NULL;
	if (*remainder != NULL)
	{
		temp = ft_strdup(*remainder);
		if (temp == NULL)
			return (free(*remainder), *remainder = NULL, NULL);
		free(*remainder);
		*remainder = NULL;
	}
	if (temp == NULL)
	{
		buffer = malloc(BUFFER_SIZE + 1);
		if (buffer == NULL)
			return (NULL);
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			return (free(buffer), NULL);
		buffer[bytes_read] = '\0';
		temp = ft_strdup(buffer);
		return (free(buffer), temp);
	}
	return (temp);
}

char	*ft_strchr(const char *string, int symbol)
{
	int	i;

	i = 0;
	while (string[i] != '\0')
	{
		if (string[i] == (unsigned char)symbol)
			return ((char *)&string[i]);
		i++;
	}
	if ((unsigned char)symbol == '\0')
		return ((char *)&string[i]);
	return (NULL);
}

// int	main(void)
// {
// 	int		fd;
// 	char	*line;

// 	fd = open("test2.txt", O_RDONLY);
// 	if (fd == -1)
// 	{
// 		perror("Error opening file");
// 		return (1);
// 	}
// 	while ((line = get_next_line(fd)) != NULL)
// 	{
// 		printf("%s", line);
// 		free(line);
// 	}
// 	close(fd);
// 	return (0);
// }
