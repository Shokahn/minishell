/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:21:58 by shokahn           #+#    #+#             */
/*   Updated: 2025/06/05 16:55:53 by stdevis          ###   ########.fr       */
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

void	true_export(char *arg, t_env **env, char *equal_sign)
{
	char	*name;
	char	*value;
	int		append;

	append = 0;
	if (equal_sign > arg && *(equal_sign - 1) == '+')
	{
		append = 1;
		name = ft_substr(arg, 0, (equal_sign - arg) - 1);
	}
	else
		name = ft_substr(arg, 0, equal_sign - arg);
	value = ft_strdup(equal_sign + 1);
	update_env(env, name, value, append);
	ft_free_str(&name);
	ft_free_str(&value);
}

void	export_without_equal(char *arg, t_env **env)
{
	char	*name;
	char	*value;

	if (!find_env(*env, arg))
	{
		name = ft_strdup(arg);
		value = NULL;
		update_env(env, name, value, 0);
		ft_free_str(&name);
	}
	else
		return ;
}

void	handle_export_assignment(char *arg, t_env **env)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
		true_export(arg, env, equal_sign);
	else
		export_without_equal(arg, env);
}

int	builtin_export(char **cmd, t_data *shell)
{
	int	i;

	if (!cmd[1])
		return (print_export(shell->env), 0);
	i = 0;
	while (cmd[++i])
	{
		if (!is_valid_identifier(cmd[i]))
		{
			shell->exit_status = 1;
			return (write(2, "export: not a valid identifier\n", 31), 1);
		}
		handle_export_assignment(cmd[i], &shell->env);
	}
	return (0);
}
