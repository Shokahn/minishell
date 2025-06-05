/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:01:46 by brcoppie          #+#    #+#             */
/*   Updated: 2025/06/05 16:58:00 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	save_fds(t_store *store)
{
	store->std_in = dup(0);
	store->std_out = dup(1);
	if (store->std_in < 0 || store->std_out < 0)
		perror("dup fail");
}

void	reset_fds(t_store *store)
{
	dup2(store->std_in, 0);
	dup2(store->std_out, 1);
	close(store->std_in);
	close(store->std_out);
}

int	is_built_in(t_cmd *cmd)
{
	int		i;
	char	*built_in_funcs[7];

	if (!cmd || !cmd->cmd || !cmd->cmd[0])
		return (0);
	i = 0;
	built_in_funcs[0] = "echo";
	built_in_funcs[1] = "cd";
	built_in_funcs[2] = "pwd";
	built_in_funcs[3] = "export";
	built_in_funcs[4] = "unset";
	built_in_funcs[5] = "env";
	built_in_funcs[6] = "exit";
	while (i <= 6)
	{
		if (ft_strcmp(cmd->cmd[0], built_in_funcs[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	exec_built_in(t_store *store, t_data *data)
{
	if (ft_strncmp(store->current->cmd[0], "echo", 5) == 0)
		data->exit_status = ft_echo(store->current->cmd);
	else if (ft_strncmp(store->current->cmd[0], "export", 7) == 0)
		data->exit_status = builtin_export(store->current->cmd, data);
	else if (ft_strncmp(store->current->cmd[0], "unset", 6) == 0)
		data->exit_status = builtin_unset(store->current->cmd, data);
	else if (ft_strncmp(store->current->cmd[0], "env", 4) == 0)
		data->exit_status = print_env(data);
	else if (ft_strncmp(store->current->cmd[0], "exit", 5) == 0)
		data->exit_status = ft_exit(data, store->current, "");
	else if (ft_strncmp(store->current->cmd[0], "cd", 3) == 0)
		data->exit_status = ft_cd(data, store->current->cmd);
	else if (ft_strncmp(store->current->cmd[0], "pwd", 4) == 0)
		data->exit_status = ft_pwd();
}
