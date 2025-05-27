/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:39:46 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/27 19:51:25 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_all_num(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(t_data *data, t_cmd *cmd)
{
	if (cmd->cmd[1])
	{
		if (cmd->cmd[2])
			fprintf(stderr, "exit: too many arguments\n");
		else if (is_all_num(cmd->cmd[1]))
		{
			ft_free_data(data);
			exit(ft_atoi(cmd->cmd[1]));
		}
		else
		{
			fprintf(stderr, "exit: argument must be numeric\n");
			exit(1);
		}
	}
	else
	{
		ft_free_data(data);
		exit(data->exit_status);
	}
}
