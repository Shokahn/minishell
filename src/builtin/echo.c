/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:16:02 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/28 13:37:22 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_args_n(char *arg)
{
	int i;

	i = 1;
	if (!arg || !arg[i])
		return (0);
	while (arg[i])
	{
		if (arg[0] != '-')
			return (0);
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo(char **args)
{
	int	i;
	int	newline_toggle;
	int first_printed;

	i = 1;
	newline_toggle = 1;
	first_printed = 0;
	while (args[i] && is_args_n(args[i]))
	{
		newline_toggle = 0;
		i++;
	}
	while (args[i])
	{
		if (args[i][0] != '\0')
		{
			if (first_printed)
				ft_putchar_fd(' ', 1);
			ft_putstr_fd(args[i], 1);
			first_printed = 1;
		}
		i++;
	}
	if (newline_toggle == 1)
		ft_putchar_fd('\n', 1);
}
