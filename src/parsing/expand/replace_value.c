/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:04:07 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/30 22:22:59 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	end_of_expansion_or_not(char *inside, int i)
{
	while (inside[i] && (ft_isalpha(inside[i]) || ft_isdigit(inside[i])
			|| inside[i] == '_'))
		i++;
	return (i);
}

char	*check_value(t_data *shell, char *name)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (!ft_strcmp(name, current->name))
			return (ft_strdup(current->inside));
		current = current->next;
	}
	return (ft_strdup(""));
}

int	replace_value(char *expand, t_token *current, int start, int end)
{
	char	*before;
	char	*after;
	char	*tmp;
	char	*joined;

	if (start <= 0)
		before = ft_strdup("");
	else
		before = ft_strndup(current->inside, start - 1);
	tmp = ft_strjoin(before, expand);
	after = ft_substr(current->inside, end, ft_strlen(current->inside) - end);
	joined = ft_strjoin(tmp, after);
	ft_free_str(&before);
	ft_free_str(&tmp);
	ft_free_str(&after);
	ft_free_str(&(current->inside));
	if (!joined)
		return (0);
	current->inside = joined;
	return (1);
}

/* int	extract_variable(char *inside, int i, t_token *current, t_data *shell)
{
	int		start;
	char	*name;
	char	*expand;

	current->expand++;
	if (inside[i] == '?')
	{
		expand = ft_itoa(shell->exit_status);
		if (!replace_value(expand, current, i, i + 1))
			return (ft_free_str(&expand), ft_exit(shell, NULL, "1"), 0);
		return (free(expand), i);
	}
	else if (ft_isalpha(inside[i]) || inside[i] == '_')
	{
		start = i;
		i = end_of_expansion_or_not(inside, i + 1);
		name = ft_substr(inside, start, i - start);
		expand = check_value(shell, name);
		if (!replace_value(expand, current, start, i))
			return (ft_free_str(&expand), ft_exit(shell, NULL, "1"), 0);
		return (ft_free_str(&name), ft_free_str(&expand), start - 1);
	}
	else if (ft_isdigit(inside[i]))
	{
		expand = ft_strdup("");
		if (!replace_value(expand, current, i, i + 1))
			return (ft_free_str(&expand), ft_exit(shell, NULL, "1"), 0);
		return (ft_free_str(&expand), i);
	}
	return (i + 1);
} */
