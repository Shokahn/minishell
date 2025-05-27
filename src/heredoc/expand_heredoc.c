/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:48:24 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/27 16:48:55 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	end_of_expansion_or_not_heredoc(char *inside, int i)
{
	while (inside[i] && (ft_isalpha(inside[i]) || ft_isdigit(inside[i])
			|| inside[i] == '_'))
		i++;
	return (i);
}

char	*check_value_heredoc(t_data *shell, char *name)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (!ft_strcmp(name, current->name))
			return (current->inside);
		current = current->next;
	}
	return (ft_strdup(""));
}

char	*replace_value_heredoc(char *expand, int start, int i, char *inside)
{
	char	*before;
	char	*after;
	char	*tmp;
	char	*joined;

	before = ft_strndup(inside, start - 1);
	tmp = ft_strjoin(before, expand);
	free(before);
	after = ft_substr(inside, i, ft_strlen(inside) - i);
	joined = ft_strjoin(tmp, after);
	free(tmp);
	free(after);
	free(inside);
	return (joined);
}

int	extract_variable_heredoc(char **inside, int i, t_data *shell)
{
	int		start;
	char	*name;
	char	*expand;
	char	*new_inside;

	if (ft_isalpha((*inside)[i]) || (*inside)[i] == '_')
	{
		start = i;
		i = end_of_expansion_or_not_heredoc(*inside, i + 1);
		name = ft_substr(*inside, start, i - start);
		expand = check_value_heredoc(shell, name);
		new_inside = replace_value_heredoc(expand, start, i, *inside);
		free(name);
		*inside = new_inside;
	}
	else if (ft_isdigit((*inside)[i]))
	{
		expand = ft_strdup("");
		new_inside = replace_value_heredoc(expand, i, i + 1, *inside);
		*inside = new_inside;
	}
	return (i + 1);
}

char	*expand_string_heredoc(char *inside, t_data *shell)
{
	int	i;

	i = 0;
	while (inside[i])
	{
		if (inside[i] == '$' && inside[i + 1])
			i = extract_variable_heredoc(&inside, i + 1, shell);
		else
			i++;
	}
	return (inside);
}
