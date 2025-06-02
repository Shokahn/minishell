/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:02:07 by stdevis           #+#    #+#             */
/*   Updated: 2025/06/02 13:48:13 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	handle_dollar(t_token *current, int i, t_data *shell)
{
	if (current->inside[i + 1] != '"' && current->inside[i + 1] != '\'')
		return (extract_variable(current->inside, i + 1, current, shell));
	return (i + 1);
}

int	expand_string(t_token *current, t_data *shell)
{
	int	i;
	int	in_the_dquote;

	i = 0;
	in_the_dquote = 0;
	while (current->inside && i < ft_strlen(current->inside)
		&& current->inside[i])
	{
		if (current->inside[i] == '\'' && i + 1 < ft_strlen(current->inside)
			&& !in_the_dquote)
			i = pass_the_quote(current->inside, i + 1, '\'');
		else if (current->inside[i] == '\"' && i
			+ 1 < ft_strlen(current->inside))
		{
			in_the_dquote = !in_the_dquote;
			i++;
		}
		else if (current->inside[i] == '$' && i + 1 < ft_strlen(current->inside)
			&& current->inside[i + 1] != '$')
			i = handle_dollar(current, i, shell);
		else
			i++;
	}
	return (1);
}

int	expandation(t_data *shell)
{
	t_token	*tmp;

	tmp = shell->token;
	while (tmp)
	{
		if (tmp->type == WORD)
		{
			if (!tmp->prev)
			{
				if (!expand_string(tmp, shell))
					return (0);
			}
			else if (tmp->prev->type != HEREDOC)
			{
				if (!expand_string(tmp, shell))
					return (0);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}
