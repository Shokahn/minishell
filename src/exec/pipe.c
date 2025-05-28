/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:08:53 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/28 17:34:59 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	open_pipe(int fd[2], t_cmd *current)
{
	if (current->next)
	{
		if (pipe(fd) == -1)
		{
			perror("pipe");
			return (0);
		}
	}
	return (1);
}

void	exec_cmd(t_store *store, t_cmd *cmd, t_data *data)
{
	char	*path;

	if ((cmd->cmd[0][0] == '/' || (cmd->cmd[0][0] == '.' && \
		cmd->cmd[0][1] == '/')) && access(cmd->cmd[0], X_OK) == 0)
		path = cmd->cmd[0];
	else
		path = find_valid_path(cmd->cmd[0], store);
	if (!path)
	{
		perror("command not found");
		ft_free_data(data);
		exit(EXIT_FAILURE);
	}
	execve(path, cmd->cmd, store->env_tab);
	perror("execve failed");
	ft_free_tab(&(store->env_tab));
	free(store);
	free(path);
	exit(EXIT_FAILURE);
}

void	launch_child(t_store *store, t_data *data)
{
	setup_sigint();
	if (store->in_fd != 0)
	{
		dup2(store->in_fd, 0);
		close(store->in_fd);
	}
	if (store->current->next)
	{
		close(store->fd[0]);
		dup2(store->fd[1], 1);
		close(store->fd[1]);
	}
	if (store->current->redir)
		handle_redirections(store->current);
	check_for_heredoc(store->current);
	if (is_built_in(store->current))
	{
		exec_built_in(store, data);
		ft_free_data(data);
		exit(EXIT_SUCCESS);
	}
	else
		exec_cmd(store, store->current, data);
}

void	handle_parent(t_store *store)
{
	pause_signals();
	if (store->in_fd != 0)
		close(store->in_fd);
	if (store->current->next)
	{
		close(store->fd[1]);
		store->in_fd = store->fd[0];
	}
	store->current = store->current->next;
}
