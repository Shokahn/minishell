/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:39:46 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/27 17:16:35 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_all_num(char *str)
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
			/*free_data(data);*/
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
		/*free_data(data);*/
		exit(data->exit_status);
	}
}
