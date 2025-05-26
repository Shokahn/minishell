/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:56:11 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/26 19:01:38 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*create_node(char *inside)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->inside = ft_strdup(inside);
	if (!new->inside) // need to free still
	{
		return (NULL);
	}
	new->next = NULL;
	new->prev = NULL;
	new->type = 0;
	new->expand = 0;
	return (new);
}

t_token	*line_to_token(t_data *shell)
{
	int		i;
	t_token	*head;
	t_token	*tail;
	t_token	*new;

	i = 0;
	head = NULL;
	tail = NULL;
	while (shell->line[i])
	{
		new = create_node(shell->line[i]);
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		else
		{
			tail->next = new;
			new->prev = tail;
		}
		tail = new;
		i++;
	}
	return (head);
}

void	define_type(t_token *current)
{
	if (!ft_strncmp(current->inside, "|", 1))
		current->type = PIPE;
	else if (!ft_strncmp(current->inside, ">>", 2))
		current->type = APPEND;
	else if (!ft_strncmp(current->inside, "<<", 2))
		current->type = HEREDOC;
	else if (!ft_strncmp(current->inside, ">", 1))
		current->type = REDIR_OUT;
	else if (!ft_strncmp(current->inside, "<", 1))
		current->type = REDIR_IN;
	else
		current->type = WORD;
}

void	identification_token(t_data *shell)
{
	t_token	*current;

	current = shell->token;
	while (current)
	{
		define_type(current);
		current = current->next;
	}
}

int	making_token(t_data *shell)
{
	shell->token = line_to_token(shell);
	if (!shell->token)
		return (0);
	identification_token(shell);
	return (1);
}
