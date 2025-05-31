/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:21:58 by shokahn           #+#    #+#             */
/*   Updated: 2025/05/31 14:13:15 by stdevis          ###   ########.fr       */
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
		if (str[i] == '+' && str[i + 1] == '=')
			break ;
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

void	update_env(t_env **env_list, char *name, char *value, int append)
{
	t_env	*existing;
	t_env	*new;
	char	*joined;

	existing = find_env(*env_list, name);
	if (existing)
	{
		if (append)
		{
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
	else
	{
		new = malloc(sizeof(t_env));
		new->name = ft_strdup(name);
		new->inside = ft_strdup(value);
		new->next = *env_list;
		*env_list = new;
	}
}

void	handle_export_assignment(char *arg, t_env **env)
{
	char	*name;
	char	*value;
	char	*equal_sign;
	int		append;

	append = 0;
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign > arg && *(equal_sign - 1) == '+')
	{
		append = 1;
		name = ft_substr(arg, 0, (equal_sign - arg) - 1);
	}
	else
		name = ft_substr(arg, 0, equal_sign - arg);
	value = ft_strdup(equal_sign + 1);
	update_env(env, name, value, append);
	free(name);
	free(value);
}

int	builtin_export(char **cmd, t_data *shell)
{
	int	i;

	if (!cmd[1])
		return (print_export(shell->env));
	i = 0;
	while (cmd[++i])
	{
		if (!is_valid_identifier(cmd[i]))
			return (write(2, "export: not a valid identifier\n", 31), 1);
		if (ft_strchr(cmd[i], '='))
			handle_export_assignment(cmd[i], &shell->env);
	}
	return (0);
}
