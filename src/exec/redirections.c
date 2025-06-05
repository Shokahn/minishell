/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:37:22 by brcoppie          #+#    #+#             */
/*   Updated: 2025/06/05 19:56:18 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	redir_in_handler(t_redir *redir, t_data *data)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		data->exit_status = 1;
		if (data->fork_check == 1)
		{
			free_env(&(data->env));
			ft_free_data(data);
			exit(EXIT_FAILURE);
		}
		else
			return (0);
	}
	dup2(fd, 0);
	close(fd);
	return (1);
}

static int	redir_out_handler(t_redir *redir, t_data *data)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		data->exit_status = 1;
		if (data->fork_check == 1)
		{
			free_env(&(data->env));
			ft_free_data(data);
			exit(EXIT_FAILURE);
		}
		else
			return (0);
	}
	dup2(fd, 1);
	close(fd);
	return (1);
}

static int	append_handler(t_redir *redir, t_data *data)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open");
		data->exit_status = 1;
		if (data->fork_check == 1)
		{
			free_env(&(data->env));
			ft_free_data(data);
			exit(EXIT_FAILURE);
		}
		else
			return (0);
	}
	dup2(fd, 1);
	close(fd);
	return (1);
}

int	handle_redirections(t_cmd *cmd, t_data *data)
{
	t_redir	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			if (!redir_in_handler(redir, data))
				return (0);
		}
		else if (redir->type == REDIR_OUT)
		{
			if (!redir_out_handler(redir, data))
				return (0);
		}
		else if (redir->type == APPEND)
		{
			if (!append_handler(redir, data))
				return (0);
		}
		redir = redir->next;
	}
	return (1);
}
