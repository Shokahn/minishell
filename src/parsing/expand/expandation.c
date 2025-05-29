/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shokahn <shokahn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:02:07 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/29 22:54:36 by shokahn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	expand_string(t_token *current, t_data *shell)
{
	int	i;
	int	in_the_dquote;

	i = 0;
	in_the_dquote = 0;
	while (i < ft_strlen(current->inside) && current->inside[i])
	{
		if (current->inside[i] == '\'' && current->inside[i + 1]
			&& in_the_dquote == 0)
			i = pass_the_quote(current->inside, i + 1, '\'');
		else if (current->inside[i] == '\"' && current->inside[i + 1])
		{
			in_the_dquote = !in_the_dquote;
			i++;
		}
		else if (current->inside[i] == '$' && current->inside[i + 1]
			&& current->inside[i + 1] != '$')
		{
			if (current->inside[i + 1] != '$')
				extract_variable(current->inside, i + 1, current, shell);
			else
				i++;
		}
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
			if (!expand_string(tmp, shell))
				return (0);
		tmp = tmp->next;
	}
	return (1);
}
