/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shokahn <shokahn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:21:58 by shokahn           #+#    #+#             */
/*   Updated: 2025/05/16 16:22:57 by shokahn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !ft_isalpha(str[0]) && str[0] != '_')
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
	t_env *existing = find_env(*env_list, name);

	if (existing)
	{
		free(existing->inside);
		existing->inside = ft_strdup(value);
	}
	else
	{
		t_env *new = malloc(sizeof(t_env));
		new->name = ft_strdup(name);
		new->inside = ft_strdup(value);
		new->next = *env_list;
		*env_list = new;
	}
}


int	builtin_export(char **args, t_data *shell)
{
	int		i;
	char	*name;
	char	*value;
	char	*equal_sign;

	if (!args[1])
		return (print_export(shell->env));
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			write(2, "export: not a valid identifier\n", 31);
			return (1);
		}
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			name = ft_substr(args[i], 0, equal_sign - args[i]);
			value = ft_strdup(equal_sign + 1);
			update_env(&shell->env, name, value);
			free(name);
			free(value);
		}
		else if (!find_env(shell->env, args[i]))
		{
			update_env(&shell->env, args[i], "");
		}
		i++;
	}
	return (0);
}
