/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:07:01 by brcoppie          #+#    #+#             */
/*   Updated: 2025/06/05 19:56:18 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_exit_status(int status, t_data *data, pid_t pid)
{
	if (WIFEXITED(status) && pid == data->store->last_pid)
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == 2)
			printf("\n");
		else if (WTERMSIG(status) == 3)
			printf("Quit (core dumped)\n");
		if (pid == data->store->last_pid)
			data->exit_status = WTERMSIG(status) + 128;
	}
}

// wait returns -1 when no children are left
static void	pickup_children(t_data *data)
{
	int		status;
	pid_t	pid;

	status = 0;
	while (1)
	{
		pid = wait(&status);
		if (pid <= 0)
			break ;
		else if (data->fork_check == 1)
			set_exit_status(status, data, pid);
	}
	close_heredoc(data->cmd);
	setup_signals();
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
			data->fork_check = 1;
			if (open_pipe(store->fd, store->current) == 0)
				return ;
			store->pid = fork();
			if (store->pid == -1)
				return (perror("fork"));
			else if (store->pid == 0)
				launch_child(store, data);
			else
			{
				store->last_pid = store->pid;
				handle_parent(store);
			}
		}
	}
	pickup_children(data);
}

static void	init_store(t_store *store, t_data *data)
{
	store->pid = -2;
	store->last_pid = -2;
	store->fd[0] = -2;
	store->fd[1] = -2;
	store->in_fd = 0;
	store->current = data->cmd;
	data->fork_check = 0;
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
