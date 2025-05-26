/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:04:07 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/26 20:01:56 by stdevis          ###   ########.fr       */
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
			return (current->inside);
		current = current->next;
	}
	return (ft_strdup(""));
}

void	replace_value(char *expand, t_token *current, int start, int i)
{
	char	*before;
	char	*after;
	char	*tmp;
	char	*joined;

	before = ft_strndup(current->inside, start - 1);
	tmp = ft_strjoin(before, expand);
	printf("before = %s\n", before);
	free(before);
	after = ft_substr(current->inside, i, ft_strlen(current->inside) - i);
	joined = ft_strjoin(tmp, after);
	printf("after = %s\n", after);
	free(tmp);
	free(after);
	free(current->inside);
	current->inside = joined;
	printf("current->inside = %s\n", current->inside);
}

int	extract_variable(char *inside, int i, t_token *current, t_data *shell)
{
	int		start;
	char	*name;
	char	*expand;

	current->expand++;
	if (ft_isalpha(inside[i]) || inside[i] == '_')
	{
		start = i;
		i = end_of_expansion_or_not(inside, i + 1);
		name = ft_substr(inside, start, i - start);
		printf(GREEN "i after expand = %d\n" RESET, i);
		printf(RED "name = %s\n" RESET, name);
		expand = check_value(shell, name);
		printf(RED "expand = %s\n" RESET, expand);
		replace_value(expand, current, start, i);
	}
	else if (ft_isdigit(inside[i]))
	{
		expand = ft_strdup("");
		replace_value(expand, current, i, i + 1);
	}
	return (i + 1);
}
