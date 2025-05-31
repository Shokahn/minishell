/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shokahn <shokahn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:27:50 by stdevis           #+#    #+#             */
/*   Updated: 2025/06/01 00:45:38 by shokahn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*add_env_node(char *str)
{
	t_env	*new;
	char	*split[2];
	int		i;
	int		start;

	i = 0;
	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
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

t_env	*add_env_node_shlvl(char *str)
{
	t_env	*new;
	char	*split[2];
	int		i;
	int		start;
	int		nbr;

	i = 0;
	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->next = NULL;
	while (str[i] && str[i] != '=')
		i++;
	split[0] = ft_substr(str, 0, i);
	start = i + 1;
	while (str[i])
		i++;
	split[1] = ft_substr(str, start, i - start);
	nbr = ft_atoi(split[1]) + 1;
	ft_free_str(&split[1]);
	split[1] = ft_itoa(nbr);
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

int	create_env_from_empty(t_data *shell)
{
	t_env	*node;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (0);
	node = add_env_node("PWD=");
	if (!node)
		return (0);
	ft_free_str(&node->inside);
	node->inside = pwd;
	link_env_node(&shell->env, node);
	node = add_env_node("SHLVL=1");
	if (!node)
		return (0);
	link_env_node(&shell->env, node);
	node = add_env_node("_=/usr/bin/env");
	if (!node)
		return (0);
	link_env_node(&shell->env, node);
	return (1);
}

t_env	*get_env(t_data *shell, char **envp)
{
	int		i;
	t_env	*first;
	t_env	*current;

	i = -1;
	first = NULL;
	if (!envp || !*envp || !**envp)
	{
		shell->env = NULL;
		if (!create_env_from_empty(shell))
			return (NULL);
		return (shell->env);
	}
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
			current = add_env_node_shlvl(envp[i]);
		else
			current = add_env_node(envp[i]);
		if (!current)
			return (NULL);
		link_env_node(&first, current);
	}
	shell->env = first;
	return (first);
}
