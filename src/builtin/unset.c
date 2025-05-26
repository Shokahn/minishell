/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:28:33 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/26 18:55:06 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	unset_variable(t_env **env, char *unset_var)
{
	t_env	*current;
	t_env	*tmp;

	current = *env;
	tmp = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, unset_var) == 0)
		{
			if (tmp == NULL)
				*env = current->next;
			else
				tmp->next = current->next;
			free(current->inside);
			free(current->name);
			free(current);
			return ;
		}
		tmp = current;
		current = current->next;
	}
}

void	builtin_unset(char **cmd, t_data *shell)
{
	int	i;

	i = 1;
	while (cmd[i])
	{
		unset_variable(&shell->env, cmd[i]);
		i++;
	}
}
