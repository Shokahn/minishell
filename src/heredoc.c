/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bri <bri@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:13:52 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/22 21:18:16 by bri              ###   ########.fr       */
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

static int	create_temp_file(char **temp_file)
{
	int	i;
	char	suffix[2];

	i = 0;
	suffix[1] = '\0';
	while (i < 10)
	{
		suffix[0] = i + '0';
		*temp_file = ft_strjoin(".heredoc_", suffix);
		if (!(*temp_file))
			return (-1);
		if (access(*temp_file, F_OK) != 0)
		{
			return (open(*temp_file, O_CREAT | O_RDWR | O_APPEND, 0600));
		}
		else
			free(*temp_file);
		i++;
	}
	return (-1);
}

void	exec_heredoc(char *delimiter, t_cmd *cmd)
{
	int		fd;
	char	*input;
	char	*tmp_file;

	fd = create_temp_file(&tmp_file);
	if (fd == -1)
		return ;
	while (1)
	{
		input = readline("> ");
		if (!input || (ft_strcmp(delimiter, input) == 0))
		{
			free(input);
			printf("\n");
			break;
		}
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	close(fd);
	cmd->redir->file = tmp_file;
	cmd->redir->fd = open(tmp_file, O_RDONLY);
}
