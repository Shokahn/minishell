/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:00:01 by stdevis           #+#    #+#             */
/*   Updated: 2025/06/05 17:06:01 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	change_underscore_env(t_data *shell)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->name, "_") == 0)
		{
			free(current->inside);
			current->inside = ft_strdup("/usr/bin/env");
		}
		current = current->next;
	}
}

int		print_env(t_data *shell)
{
	t_env	*tmp;

	tmp = shell->env;
	change_underscore_env(shell);
	while (tmp)
	{
		if (tmp->inside != NULL)
		{
			printf("%s=", tmp->name);
			printf("%s\n", tmp->inside);
		}
		tmp = tmp->next;
	}
	return (0);
}
