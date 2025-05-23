/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:21:58 by shokahn           #+#    #+#             */
/*   Updated: 2025/05/23 17:22:07 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

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

void	update_env(t_env **env_list, char *name, char *value)
{
	t_env	*existing;
	t_env	*new;

	existing = find_env(*env_list, name);
	if (existing)
	{
		free(existing->inside);
		existing->inside = ft_strdup(value);
	}
	else
	{
		new = malloc(sizeof(t_env));
		new->name = ft_strdup(name);
		new->inside = ft_strdup(value);
		new->next = *env_list;
		*env_list = new;
	}
}

void	ft_sort_env_tab(char **env_tab)
{
	int	i;
	int	sorted;

	sorted = 0;
	while (!sorted)
	{
		i = 0;
		sorted = 1;
		while (env_tab[i + 1])
		{
			if (ft_strcmp(env_tab[i], env_tab[i + 1]) > 0)
			{
				ft_swap(&env_tab[i], &env_tab[i + 1]);
				sorted = 0;
			}
			i++;
		}
	}
}

int has_equal(char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

char	*fill_the_env_quoted(char *str)
{
	char	*env;
	int		len;
	int i;
	int j;

	i = 0;
	j = 0;
	if (!has_equal(str))
		return (ft_strdup(str));
	len = ft_strlen(str);
	env = malloc(sizeof(char) * (len + 3));
	if (!env)
		return (NULL);
	while(str[i] && str[i] != '=')
		env[j++] = str[i++];
	env[j++] = '=';
	env[j++] = '"';
	i++;
	while(str[i])
		env[j++] = str[i++];
	env[j++] = '"';
	env[j] = '\0';
	return (env);
}

char	**quoting_the_inside(char **env_tab)
{
	int		len;
	char	**env_quoted;
	int		i;

	i = 0;
	len = ft_strlen_tab(env_tab);
	env_quoted = malloc(sizeof(char *) * (len + 1));
	if (!env_quoted)
		return (NULL);
	while (i < len)
	{
		env_quoted[i] = fill_the_env_quoted(env_tab[i]);
		if (!env_quoted[i])
			return (ft_free_index(env_quoted, i), NULL);
		i++;
	}
	env_quoted[i] = NULL;
	ft_free_tab(env_tab);
	return(env_quoted);
}

int	print_export(t_env *env)
{
	char	**env_tab;
	char	**env_quoted;
	int		i;

	env_tab = ft_list_to_tab(env);
	if (!env_tab)
		return (0);
	ft_sort_env_tab(env_tab);
	i = 0;
	env_quoted = quoting_the_inside(env_tab);
	if (!env_quoted)
		return (0);
	i = 0;
	while (env_quoted[i])
	{
		printf("declare -x %s\n", env_quoted[i]);
		i++;
	}
	return (1);
}

int	builtin_export(char **cmd, t_data *shell)
{
	int		i;
	char	*name;
	char	*value;
	char	*equal_sign;

	if (!cmd[1])
		return (print_export(shell->env));
	i = 1;
	while (cmd[i])
	{
		if (!is_valid_identifier(cmd[i]))
		{
			write(2, "export: not a valid identifier\n", 31);
			return (1);
		}
		equal_sign = ft_strchr(cmd[i], '=');
		if (equal_sign)
		{
			name = ft_substr(cmd[i], 0, equal_sign - cmd[i]);
			value = ft_strdup(equal_sign + 1);
			update_env(&shell->env, name, value);
			free(name);
			free(value);
		}
		else
			update_env(&shell->env, name, NULL);
		i++;
	}
	return (0);
}
