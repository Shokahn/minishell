/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shokahn <shokahn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:13:52 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/29 17:23:01 by shokahn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_heredoc(t_cmd *cmd)
{
	t_redir	*redir;

	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir && redir->type == HEREDOC && redir->fd != -1)
			{
				close(redir->fd);
				unlink(redir->file);
				ft_free_str(&(redir->file));
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

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
			ft_free_str(temp_file);
		i++;
	}
	return (-1);
}

void	exec_heredoc(char *delimiter, t_redir *redir, t_data *data)
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
		(write(fd, input, ft_strlen(input)), write(fd, "\n", 1));
		free(input);
	}
	close(fd);
	ft_free_str(&(redir->file));
	redir->file = tmp_file;
	redir->fd = open(tmp_file, O_RDONLY);
	setup_signals();
}

void	check_for_heredoc(t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir && redir->type == HEREDOC && redir->fd != -1)
		{
			dup2(redir->fd, 0);
			close(redir->fd);
		}
		redir = redir->next;
	}
}

int	count_heredocs(t_data *data)
{
	t_cmd	*cmd;
	t_redir	*redir;
	int		i;

	i = 0;
	cmd = data->cmd;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir && redir->type == HEREDOC)
				i++;
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (i);
}

void	init_heredoc(t_data *data)
{
	t_cmd	*cmd;
	t_redir	*redir;
	int		i;

	i = 0;
	cmd = data->cmd;
	if (count_heredocs(data) > 9)
	{
		printf("maximum here-document count exceeded\n");
		ft_exit(data, data->cmd, "2");
	}
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir && redir->type == HEREDOC)
			{
				exec_heredoc(redir->file, redir, data);
				i++;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}
