/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:21:58 by shokahn           #+#    #+#             */
/*   Updated: 2025/05/21 18:52:28 by stdevis          ###   ########.fr       */
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

t_env	*copy_env(t_env *env)
{
	t_env	*copy;
	t_env	*tmp;

	while (env)
	{
		tmp = malloc(sizeof(t_env));
		if (!tmp)
			return (NULL);
		tmp->name = ft_strdup(env->name);
		if (!tmp->inside)
			tmp->inside = NULL;
		tmp->inside = ft_strdup(env->inside);
		tmp->next = copy;
		copy = tmp;
		env = env->next;
	}
	return (copy);
}

void	swap_env(t_env *a, t_env *b)
{
	char	*tmp_name;
	char	*tmp_inside;

	tmp_name = a->name;
	tmp_inside = a->inside;
	a->name = b->name;
	b->name = tmp_name;
	a->inside = b->inside;
	b->inside = tmp_inside;
}

void	sort_env(t_env *env)
{
	t_env	*i;
	t_env	*j;

	i = env;
	while (i && i->next)
	{
		j = i->next;
		while (j && i && j->name && i->name)
		{
			
			printf("i name = %s | j name = %s\n", i->name, j->name);
			if (ft_strcmp(i->name, j->name) > 0)
				swap_env(i, j);
			j = j->next;
		}
		i = i->next;
	}
}

void	free_copy(t_env *tmp)
{
	t_env *next;

	next = tmp->next;
	while (tmp)
	{
		free(tmp->name);
		if (tmp->inside)
			free(tmp->inside);
		free(tmp);
		tmp = next;
	}
}

int	print_export(t_env *env)
{
	t_env	*sorted;
	t_env	*tmp;

	sorted = copy_env(env);
	tmp = sorted;
	if (!sorted)
		return (1);
	sort_env(sorted);
	while (sorted)
	{
		if (sorted->inside)
			printf("declare -x %s=\"%s\"\n", sorted->name, sorted->inside);
		else
			printf("declare -x %s\n", sorted->name);
		sorted = sorted->next;
	}
	free_copy(tmp);
	return (0);
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
