/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:13:52 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/21 19:24:14 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//use softer version of expand
/*static int	expand_string(t_token *current, t_data *shell)
{
	int	i;
	int	check;

	i = 0;
	check = 0;
	while (current->inside[i])
	{
		if (current->inside[i] == '$' && current->inside[i + 1])
			i = extract_variable(current->inside, i + 1, current, shell);
		else
			i++;
	}
	return (1);
}*/

static int	fill_pipe(char *input, int fd)
{
	int	counter;

	counter = ft_strlen(input) + 1;
	write(fd, input, counter);
	write(fd, "\n", 1);
	return (counter);
}

static char	*read_pipe(int counter, int fd)
{
	char	*result;
	int		chars_read;

	result = malloc(counter + 1);
	chars_read = read(fd, result, counter);
	if (chars_read == -1)
	{
		free(result);
		return (NULL);
	}
	result[counter] = '\0';
	return (result);
}

char	*exec_heredoc(char *delimiter)
{
	int		fd[2];
	int		counter;
	char	*input;
	char	*result;

	counter = 0;
	if (pipe(fd) == -1)
		return ("pipe not working");
	while (1)
	{
		input = readline("> ");
		if (!input || (ft_strcmp(delimiter, input) == 0))
		{
			printf("\n");
			break;
		}
		counter += fill_pipe(input, fd[1]);
		free(input);
	}
	close(fd[1]);
	result = read_pipe(counter, fd[0]);
	return (result);
}

