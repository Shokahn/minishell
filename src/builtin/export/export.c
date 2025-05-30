/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:21:58 by shokahn           #+#    #+#             */
/*   Updated: 2025/05/30 13:29:09 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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

int	builtin_export(char **cmd, t_data *shell)
{
	int		i;
	char	*name;
	char	*value;
	char	*equal_sign;

	if (!cmd[1])
		return (print_export(shell->env));
	i = 0;
	while (cmd[++i])
	{
		if (!is_valid_identifier(cmd[i]))
			return (write(2, "export: not a valid identifier\n", 31), 1);
		equal_sign = ft_strchr(cmd[i], '=');
		if (equal_sign)
		{
			name = ft_substr(cmd[i], 0, equal_sign - cmd[i]);
			value = ft_strdup(equal_sign + 1);
			update_env(&shell->env, name, value);
			free(name);
			free(value);
		}
	}
	return (0);
}
