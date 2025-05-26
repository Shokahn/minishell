/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:20:30 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/26 19:01:20 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	pipe_startend(t_data *shell)
{
	t_token	*current;

	current = shell->token;
	if (current->type == 1)
		return (0);
	while (current)
	{
		if (current->next == NULL)
		{
			if (current->type == 1)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	no_file_after_redir(t_data *shell)
{
	t_token	*current;

	current = shell->token;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == APPEND || current->type == HEREDOC)
		{
			if (!current->next)
				return (0);
			else if (current->next->type == 0)
				current = current->next;
			else
				return (0);
		}
		else
			current = current->next;
	}
	return (1);
}

int	parsing(t_data *shell)
{
	if (shell->sep[0] == 4)
		return (ft_error(shell, 0,
				"minishell : syntax error quote in unquoted cell\n"), 0);
	if (!(pipe_startend(shell)))
		return (ft_error(shell, 0,
				"minishell : syntax error near unexpected token '|'\n"), 0);
	if (!(no_file_after_redir(shell)))
		return (ft_error(shell, 0,
				"minishell: syntax error near unexpected token 'newline'\n"),
			0);
	token_cleaning(shell);
	if (!(expand_token_recuting(shell)))
		return (0);
	return (1);
}
