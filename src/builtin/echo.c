/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:16:02 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/26 18:55:22 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_echo(char **args)
{
	int	i;
	int	newline_toggle;

	i = 1;
	newline_toggle = 1;
	if (args[1] && ft_strncmp(args[1], "-n", 3) == 0)
	{
		newline_toggle = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		i++;
	}
	if (newline_toggle == 1)
		ft_putchar_fd('\n', 1);
}
