/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:39:46 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/31 13:09:48 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_all_num(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(t_data *data, t_cmd *cmd, char *force_status)
{
	if (ft_strcmp(force_status, "") != 0)
		data->exit_status = ft_atoi(force_status);
	else if (cmd->cmd[1])
	{
		if (cmd->cmd[2])
			fprintf(stderr, "exit: too many arguments\n");
		else if (is_all_num(cmd->cmd[1]))
			data->exit_status = ft_atoi(cmd->cmd[1]);
		else
		{
			fprintf(stderr, "exit: argument must be numeric\n");
			free_env(&(data->env));
			ft_free_data(data);
			printf("exit\n");
			exit(1);
		}
	}
	printf("exit\n");
	free_env(&(data->env));
	ft_free_data(data);
	if (data->exit_status > 255 || data->exit_status <= 0)
		exit(1);
	exit(data->exit_status);
}
