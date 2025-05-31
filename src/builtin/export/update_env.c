/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shokahn <shokahn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:50:59 by shokahn           #+#    #+#             */
/*   Updated: 2025/05/31 23:22:53 by shokahn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_env	*find_env(t_env *env, char *name)
{
	while (env)
	{
		if (!ft_strcmp(env->name, name))
			return (env);
		env = env->next;
	}
	return (NULL);
}

static void	update_existing_env(t_env *existing, char *value, int append)
{
	char	*joined;

	if (append)
	{
		if (!existing->inside)
			joined = ft_strdup(value);
		else
			joined = ft_strjoin(existing->inside, value);
		ft_free_str(&(existing->inside));
		existing->inside = joined;
	}
	else
	{
		ft_free_str(&(existing->inside));
		existing->inside = ft_strdup(value);
	}
}

static void	add_new_env(t_env **env_list, char *name, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->name = ft_strdup(name);
	new->inside = ft_strdup(value);
	new->next = *env_list;
	*env_list = new;
}

void	update_env(t_env **env_list, char *name, char *value, int append)
{
	t_env	*existing;

	existing = find_env(*env_list, name);
	if (existing)
		update_existing_env(existing, value, append);
	else
		add_new_env(env_list, name, value);
}

char	**ft_list_to_tab_export(t_env *env)
{
	char	**tab;
	int		i;

	i = 0;
	tab = malloc(sizeof(char *) * (ft_envsize(env) + 1));
	if (!tab)
		return (NULL);
	while (env)
	{
		if (env->inside == NULL)
		{
			tab[i] = ft_strdup(env->name);
		}
		else
		{
			tab[i] = fill_tab(env);
			if (!tab[i])
				return (ft_free_tab(&tab), NULL);
		}
		i++;
		env = env->next;
	}
	tab[i] = 0;
	return (tab);
}
