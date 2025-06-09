/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:39:46 by brcoppie          #+#    #+#             */
/*   Updated: 2025/06/09 11:14:54 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	convert_exit_status(long status)
{
	if (status > 0)
		return (status % 256);
	if (status < 0)
		return (status % 256 + 256);
	return (0);
}

static void	ft_free_everything_and_exit(t_data *data, int i, int check)
{
	write(2, "exit\n", 5);
	if (check == 0)
		ft_putstr_fd("exit: too many arguments\n", 2);
	if (check == 1)
		ft_putstr_fd("exit: argument must be numeric\n", 2);
	errno = 0;
	if (check != 0)
	{
		free_env(&(data->env));
		ft_free_data(data);
		exit(i);
	}
}

int	count_no_empty_arg(char **cmd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cmd[i])
	{
		if (cmd[i][0])
			count++;
		i++;
	}
	return (count);
}

char	**arg_without_empty(char **cmd)
{
	int		i;
	int		j;
	int		count;
	char	**noempty_cmd;

	count = 0;
	count = count_no_empty_arg(cmd);
	noempty_cmd = malloc(sizeof(char *) * (count + 1));
	if (!noempty_cmd)
		return (NULL);
	i = -1;
	j = 0;
	while (cmd[++i])
	{
		if (cmd[i][0])
		{
			noempty_cmd[j] = ft_strdup(cmd[i]);
			if (!noempty_cmd)
				return (ft_free_index(&noempty_cmd, j), NULL);
			j++;
		}
	}
	noempty_cmd[j] = NULL;
	ft_free_tab(&cmd);
	return (noempty_cmd);
}

int	ft_exit(t_data *data, t_cmd *cmd, char *force_status)
{
	cmd->cmd = arg_without_empty(cmd->cmd);
	if (!cmd->cmd)
		return (ft_free_data(data), free_env(&(data->env)), exit(1), 1);
	if (ft_strcmp(force_status, "") != 0)
		data->exit_status = ft_atoi(force_status);
	if (cmd->cmd[1])
	{
		if (is_all_num(cmd->cmd[1]))
		{
			data->exit_status = ft_atol(cmd->cmd[1]);
			if (errno == ERANGE)
				ft_free_everything_and_exit(data, 2, 1);
		}
		else
			ft_free_everything_and_exit(data, 2, 1);
		if (cmd->cmd[2])
			return (ft_free_everything_and_exit(data, 1, 0), 1);
	}
	if (data->exit_status < 0 || data->exit_status > 255)
		data->exit_status = convert_exit_status(data->exit_status);
	write(2, "exit\n", 5);
	free_env(&(data->env));
	ft_free_data(data);
	exit(data->exit_status);
	return (1);
}
