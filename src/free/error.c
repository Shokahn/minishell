/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:56:26 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/30 16:56:58 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_env(t_env **env)
{
	t_env	*current;
	t_env	*tmp;

	if (!env | !*env)
		return ;
	current = *env;
	while (current)
	{
		tmp = current->next;
		ft_free_str(&(current->inside));
		ft_free_str(&(current->name));
		free(current);
		current = tmp;
	}
	*env = NULL;
}

void	ft_error(int type, char *message)
{
	if (type == 0)
	{
		ft_putstr_fd(ITALIC RED BOLD "Error : " RESET, 2);
		ft_putstr_fd(message, 2);
	}
	else if (type == 1)
		perror(message);
}
