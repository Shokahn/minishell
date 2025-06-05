/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:07:01 by brcoppie          #+#    #+#             */
/*   Updated: 2025/06/05 15:24:50 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// wait returns -1 when no children are left
static void	pickup_children(t_data *data)
{
	int	status;

	status = 0;
	while (wait(&status) > 0)
		;
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_status = WTERMSIG(status) + 128;
	close_heredoc(data->cmd);
	setup_signals();
}

static int	exec_builtin_cmds(t_store *store, t_data *data)
{
	if (store->current->redir)
	{
		save_fds(store);
		if (!handle_redirections(store->current, data))
		{
			reset_fds(store);
			store->current = store->current->next;
			return (0);
		}
		check_for_heredoc(store->current);
	}
	exec_built_in(store, data);
	if (store->current->redir)
		reset_fds(store);
	store->current = store->current->next;
	return (1);
}

static void	exec_cmds(t_store *store, t_data *data)
{
	if (store->current && store->current->cmd)
	{
		if (is_built_in(store->current) && !store->current->next)
		{
			if (!exec_builtin_cmds(store, data))
				return ;
		}
		while (store->current && store->current->cmd)
		{
			if (open_pipe(store->fd, store->current) == 0)
				return ;
			store->pid = fork();
			if (store->pid == -1)
				return (perror("fork"));
			else if (store->pid == 0)
			{
				launch_child(store, data);
				data->builtin_check = 1;
			}
			else
				handle_parent(store);
		}
	}
	pickup_children(data);
}

static void	init_store(t_store *store, t_data *data)
{
	store->pid = -2;
	store->fd[0] = -2;
	store->fd[1] = -2;
	store->in_fd = 0;
	store->current = data->cmd;
	store->env_tab = ft_list_to_tab(data->env);
}

void	setup_exec(t_data *data)
{
	data->store = malloc(sizeof(t_store));
	if (!data->store)
	{
		ft_error(0, "Failed to allocate memory for store\n");
		ft_exit(data, data->cmd, "1");
	}
	init_store(data->store, data);
	init_heredoc(data);
	exec_cmds(data->store, data);
}
