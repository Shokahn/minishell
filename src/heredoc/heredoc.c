/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:13:52 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/27 16:48:50 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	create_temp_file(char **temp_file)
{
	int		i;
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

void	exec_heredoc(char *delimiter, t_cmd *cmd, t_data *data)
{
	int		fd;
	char	*input;
	char	*tmp_file;

	pause_signals();
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
			break ;
		}
		input = expand_string_heredoc(input, data);
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	close(fd);
	cmd->redir->file = tmp_file;
	cmd->redir->fd = open(tmp_file, O_RDONLY);
	setup_signals();
}
