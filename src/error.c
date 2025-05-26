/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:09:26 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/26 17:23:00 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_error(t_data *shell, int type, char *message)
{
	(void)shell;
	if (type == 0)
	{
		ft_putstr_fd(ITALIC RED BOLD "Error : " RESET, 2);
		ft_putstr_fd(message, 2);
	}
	else if (type == 1)
		perror(message);
}
