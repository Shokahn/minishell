/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_cleaning.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:25:02 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/30 22:34:01 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	pass_the_quote(char *inside, int i, char c)
{
	while (inside[i] && inside[i] != c)
		i++;
	return (i + 1);
}

char	*remove_quote(t_token *current)
{
	int		i;
	int		j;
	int		in_single;
	int		in_double;
	char	*result;

	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (current->inside[i])
		i++;
	result = malloc(sizeof(char) * (i + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (current->inside[i])
	{
		if (current->inside[i] == '\'' && in_double == 0)
			in_single = !in_single;
		else if (current->inside[i] == '\"' && in_single == 0)
			in_double = !in_double;
		else
			result[j++] = current->inside[i];
		i++;
	}
	result[j] = '\0';
	ft_free_str(&(current->inside));
	return (result);
}

int	token_cleaning(t_data *shell)
{
	t_token	*current;

	current = shell->token;
	while (current)
	{
		current->inside = remove_quote(current);
		if (!current->inside)
			return (0);
		current = current->next;
	}
	return (1);
}
