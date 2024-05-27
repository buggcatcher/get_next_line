/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mailinci <mailinci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 17:35:38 by mailinci          #+#    #+#             */
/*   Updated: 2024/05/27 17:50:13 by mailinci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*fd_reader(int fd, int *flag)
{
	int		char_read;
	char	*buffer;

	buffer = ft_calloc_bzero(sizeof(char), BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	char_read = read(fd, buffer, BUFFER_SIZE);
	if (char_read == 0)
	{
		free(buffer);
		return (NULL);
	}
	else if (char_read == -1)
	{
		free(buffer);
		*flag = 1;
		return (NULL);
	}
	return (buffer);
}

char	*extract_from_buffer(char **lbuffer)
{
	char	*line;
	char	*char_left;
	int		x;

	x = 0;
	if (!lbuffer || !*lbuffer)
		return (NULL);
	while ((*lbuffer)[x] != '\n' && (*lbuffer)[x] != '\0')
		x++;
	if ((*lbuffer)[x] == '\n')
	{
		line = ft_substr(*lbuffer, 0, x + 1);
		if (!line)
			return (NULL);
		char_left = ft_substr(*lbuffer, x + 1, ft_strlen(*lbuffer) - (x + 1));
		if (!char_left)
		{
			free(line);
			return (NULL);
		}
		free(*lbuffer);
		*lbuffer = char_left;
		return (line);
	}
	return (NULL);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new;
	size_t	strlen;
	size_t	end;

	if (!s)
		return (0);
	strlen = ft_strlen(s);
	end = 0;
	if (start <= strlen)
		end = strlen - start;
	if (start > strlen)
		return (ft_calloc_bzero(1, sizeof(char)));
	if (end > len)
		end = len;
	new = (char *)ft_calloc_bzero(end + 1, sizeof(char));
	if (!new)
		return (0);
	ft_strlcpy(new, s + start, end + 1);
	return (new);
}

char	*read_and_assemble(char **lbuffer, int fd, int *flag)
{
	char	*read_line;
	char	*tmp;
	char	*line;

	line = extract_from_buffer(lbuffer);
	if (line)
		return (line);
	read_line = fd_reader(fd, flag);
	if (!read_line)
	{
		if (*flag == 1)
			return (NULL);
		line = ft_strdup(*lbuffer);
		free(*lbuffer);
		*lbuffer = NULL;
		if (*line)
			return (line);
		free (line);
		return (NULL);
	}
	tmp = ft_strjoin(*lbuffer, read_line);
	free(*lbuffer);
	free(read_line);
	*lbuffer = tmp;
	return (read_and_assemble(lbuffer, fd, flag));
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	int			flag;
	char		*result;

	flag = 0;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buffer)
		buffer = ft_strdup("");
	result = read_and_assemble(&buffer, fd, &flag);
	if (flag)
	{
		free(buffer);
		buffer = (NULL);
		return (NULL);
	}
	return (result);
}

/*  TEST PER READ E BUFFER

int main(void)
{
    int fd;
    char *buffer;
    int n;

    fd = open("lorem.txt", O_RDONLY);
    n = 1;
    if (fd < 0)
    {
        perror("Error opening file\n");
        return 1;
    }
    while ((buffer = fd_reader(fd)) != NULL)
    {
        printf("linea %d: %s\n", n, buffer);
        n++;
        free(buffer);
    }
    close(fd);
    return 0;
}*/

/*  TEST PER SUBSTR

int main()
{
    char *s = "Hello, world!";
    unsigned int start = 7;
    size_t len = 5;
    char *substr = ft_substr(s, start, len);

    if (substr)
    {
        printf("Substring: %s\n", substr);
        free(substr);
    }
    else
    {
        printf("ft_substr failed to allocate memory\n");
    }

    return 0;
}*/

/*  TEST PER READ E ASSEMBLE

int main()
{
    int fd = open("test.txt", O_RDONLY);
    char *buffer = NULL;
    char *line;

    if (fd < 0)
    {
        perror("Error opening file\n");
        return 1;
    }

    while ((line = read_and_assemble(&buffer, fd)) != NULL)
    {
        printf("Line: %s\n", line);
        free(line);
    }

    free(buffer);
    close(fd);
    return 0;
}*/

// int main()
// {
//     int fd = open("lorem.txt", O_RDONLY);
//     char *line;
//     int i = 1;

//     if (fd < 0)
//     {
//         perror("Error opening file");
//         return 1;
//     }

//     while ((line = get_next_line(fd)) != NULL)
//     {
//         printf("Linea %d: %s",i ,line);
//         i++;
//         free(line);
//     }

//     close(fd);
//     return 0;
// }