/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shokahn <shokahn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:04:07 by stdevis           #+#    #+#             */
/*   Updated: 2025/06/08 19:26:35 by shokahn          ###   ########.fr       */
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
