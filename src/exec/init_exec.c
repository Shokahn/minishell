/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:07:01 by brcoppie          #+#    #+#             */
/*   Updated: 2025/06/04 20:32:37 by stdevis          ###   ########.fr       */
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
	data->exit_status = WEXITSTATUS(status);
	g_sigint_catcher = 0;
	close_heredoc(data->cmd);
	setup_signals();
}

static void	exec_builtin_cmds(t_store *store, t_data *data)
{
	if (store->current->redir)
	{
		save_fds(store);
		handle_redirections(store->current, data);
		check_for_heredoc(store->current);
	}
	exec_built_in(store, data);
	if (store->current->redir)
		reset_fds(store);
	store->current = store->current->next;
}

static void	exec_cmds(t_store *store, t_data *data)
{
	if (store->current && store->current->cmd)
	{
		if (is_built_in(store->current) && !store->current->next)
			exec_builtin_cmds(store, data);
		while (store->current && store->current->cmd)
		{
			if (open_pipe(store->fd, store->current) == 0)
				return ;
			store->pid = fork();
			if (store->pid == -1)
			{
				perror("fork");
				return ;
			}
			else if (store->pid == 0)
				launch_child(store, data);
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
	if (data->exit_status == 130)
		return ;
	exec_cmds(data->store, data);
}
