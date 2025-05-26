/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_cmd_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:35:10 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/26 19:59:27 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	count_args(t_token *start, t_token *end)
{
	t_token	*tmp;
	int		count;

	tmp = start;
	count = 0;
	while (tmp && tmp != end)
	{
		if (tmp->type == WORD)
			count++;
		else if (tmp->type != WORD)
			count--;
		tmp = tmp->next;
	}
	return (count);
}

char	**collect_cmd_args(t_token *start, t_token *end)
{
	char	**args;
	int		count_arg;
	int		i;
	int		check;

	i = 0;
	check = 0;
	count_arg = count_args(start, end);
	args = malloc(sizeof(char *) * (count_arg + 1));
	if (!args)
		return (NULL);
	while (start && start != end)
	{
		if (start->type == WORD && (check == 0 || (start->prev
					&& start->prev->type != REDIR_IN
					&& start->prev->type != REDIR_OUT
					&& start->prev->type != APPEND
					&& start->prev->type != HEREDOC)))
			args[i++] = ft_strdup(start->inside);
		start = start->next;
		check++;
	}
	args[i] = NULL;
	return (args);
}
