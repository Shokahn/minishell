/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 17:00:15 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/30 17:02:27 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	create_temp_file(char **temp_file)
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

static int	count_heredocs(t_data *data)
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
