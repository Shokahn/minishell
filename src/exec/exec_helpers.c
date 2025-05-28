/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:39:59 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/28 17:17:10 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_envsize(t_env *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

char	**ft_list_to_tab(t_env *env)
{
	char	**tab;
	char	*tmp;
	char	*join;
	int		i;

	i = 0;
	if (!env)
		return (NULL);
	tab = malloc(sizeof(char *) * (ft_envsize(env) + 1));
	if (!tab)
		return (NULL);
	while (env)
	{
		if (env->inside == NULL)
			env = env->next;
		tmp = ft_strjoin(env->name, "=");
		if (!tmp)
			return (ft_free_tab(&tab), NULL);
		join = ft_strjoin(tmp, env->inside);
		free(tmp);
		if (!join)
			return (ft_free_tab(&tab), NULL);
		tab[i] = join;
		if (!tab[i])
			return (ft_free_tab(&tab), NULL);
		i++;
		env = env->next;
	}
	tab[i] = 0;
	return (tab);
}

char	**ft_tab_dup(char **tab)
{
	char	**copy;
	int		i;
	int		l;

	if (!tab || !tab[0])
		return (NULL);
	l = 0;
	while (tab[l])
		l++;
	copy = malloc(sizeof(char *) * (l + 1));
	i = 0;
	while (tab[i])
	{
		copy[i] = ft_strdup(tab[i]);
		if (!copy[i])
		{
			ft_free_tab(&copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = 0;
	return (copy);
}

static char	**get_paths(char **env)
{
	int		i;
	char	**result;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
		{
			result = ft_split(env[i] + 5, ':');
			return (result);
		}
		i++;
	}
	return (NULL);
}

char	*find_valid_path(const char *str, t_store *store)
{
	char	**paths;
	int		i;
	char	*pathname;
	char	*tmp;

	paths = get_paths(store->env_tab);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		pathname = ft_strjoin(tmp, str);
		if (access(pathname, X_OK) == 0)
		{
			ft_free_tab(&paths);
			return (pathname);
		}
		free(tmp);
		free(pathname);
		i++;
	}
	ft_free_tab(&paths);
	return (NULL);
}
