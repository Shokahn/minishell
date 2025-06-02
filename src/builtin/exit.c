/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:39:46 by brcoppie          #+#    #+#             */
/*   Updated: 2025/06/02 11:37:00 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_all_num(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

static int	convert_exit_status(int status)
{
	if (status < 0)
		status = convert_exit_status(status + 256);
	if (status > 255)
		status = convert_exit_status(status - 256);
	return (status);
}

static void	ft_free_everything_and_exit_1(t_data *data)
{
	free_env(&(data->env));
	ft_free_data(data);
	printf("exit\n");
	exit(1);
}

void	ft_exit(t_data *data, t_cmd *cmd, char *force_status)
{
	if (ft_strcmp(force_status, "") != 0)
		data->exit_status = ft_atoi(force_status);
	else if (cmd->cmd[1])
	{
		if (cmd->cmd[2])
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			ft_free_everything_and_exit_1(data);
		}
		else if (is_all_num(cmd->cmd[1]))
			data->exit_status = ft_atoi(cmd->cmd[1]);
		else
		{
			ft_putstr_fd("exit: argument must be numeric\n", 2);
			ft_free_everything_and_exit_1(data);
		}
	}
	printf("exit\n");
	free_env(&(data->env));
	ft_free_data(data);
	if (data->exit_status < 0 || data->exit_status > 255)
		data->exit_status = convert_exit_status(data->exit_status);
	exit(data->exit_status);
}
