/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:27:50 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/30 15:01:04 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*add_env_node(char *str, t_data *shell)
{
	t_env	*new;
	char	*split[2];
	int		i;
	int		start;

	(void)shell;
	i = 0;
	new = malloc(sizeof(t_env));
	if (!new)
		return (ft_error(0, "malloc failed\n"), NULL);
	new->next = NULL;
	while (str[i] && str[i] != '=')
		i++;
	split[0] = ft_substr(str, 0, i);
	start = i + 1;
	while (str[i])
		i++;
	split[1] = ft_substr(str, start, i - start);
	new->name = split[0];
	new->inside = split[1];
	return (new);
}

void	link_env_node(t_env **first, t_env *new)
{
	t_env	*tmp;

	if (!(*first))
	{
		*first = new;
		return ;
	}
	tmp = *first;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_env	*get_env(t_data *shell, char **envp)
{
	int		i;
	t_env	*first;
	t_env	*current;

	i = 0;
	first = NULL;
	while (envp[i])
	{
		current = add_env_node(envp[i], shell);
		if (!current)
			return (0);
		link_env_node(&first, current);
		i++;
	}
	return (first);
}
