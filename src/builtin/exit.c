/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:39:46 by brcoppie          #+#    #+#             */
/*   Updated: 2025/06/05 19:10:33 by stdevis          ###   ########.fr       */
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

int	ft_exit(t_data *data, t_cmd *cmd, char *force_status)
{
	if (ft_strcmp(force_status, "") != 0)
		data->exit_status = ft_atoi(force_status);
	if (cmd->cmd[1])
	{
		if (cmd->cmd[2])
			ft_free_everything_and_exit(data, 1, 0);
		else if (is_all_num(cmd->cmd[1]))
		{
			data->exit_status = ft_atol(cmd->cmd[1]);
			if (errno == ERANGE)
				ft_free_everything_and_exit(data, 2, 1);
		}
		else
			ft_free_everything_and_exit(data, 1, 1);
	}
	if (data->exit_status < 0 || data->exit_status > 255)
		data->exit_status = convert_exit_status(data->exit_status);
	if (!cmd->cmd[1])
	{
		write(2, "exit\n", 5);
		free_env(&(data->env));
		ft_free_data(data);
		exit(data->exit_status);
	}
	return (1);
}
