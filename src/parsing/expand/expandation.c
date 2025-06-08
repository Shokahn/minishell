/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shokahn <shokahn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:02:07 by stdevis           #+#    #+#             */
/*   Updated: 2025/06/08 19:25:10 by shokahn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	expand_string(t_token *current, t_data *shell)
{
	int	i;

	i = 0;
	shell->in_the_quote = 0;
	while (current->inside && i < ft_strlen(current->inside)
		&& current->inside[i])
	{
		if (current->inside[i] == '\'' && i + 1 < ft_strlen(current->inside)
			&& !shell->in_the_quote)
			i = pass_the_quote(current->inside, i + 1, '\'');
		else if (current->inside[i] == '\"' && i
			+ 1 < ft_strlen(current->inside))
		{
			shell->in_the_quote = !shell->in_the_quote;
			i++;
		}
		else if (current->inside[i] == '$' && i + 1 < ft_strlen(current->inside)
			&& current->inside[i + 1] != '$')
			i = extract_variable(current->inside, i + 1, current, shell);
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
