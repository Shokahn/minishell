/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   making_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:33:11 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/30 17:15:19 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_redir	*init_redir(t_token *token)
{
	t_redir	*redir;

	if (!token || !token->next)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = token->type;
	redir->file = strdup(token->next->inside);
	redir->next = NULL;
	redir->fd = -1;
	return (redir);
}

void	add_redir(t_redir **redir_list, t_redir *new)
{
	t_redir	*tmp;

	if (!*redir_list)
		*redir_list = new;
	else
	{
		tmp = *redir_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->next = NULL;
	cmd->redir = NULL;
	return (cmd);
}

t_token	*fill_cmd(t_token *token, t_cmd *cmd)
{
	t_token	*start;
	t_token	*tmp;
	t_redir	*redir;

	start = token;
	tmp = token;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == REDIR_IN || tmp->type == REDIR_OUT
			|| tmp->type == APPEND || tmp->type == HEREDOC)
		{
			redir = init_redir(tmp);
			if (!redir)
				return (NULL);
			add_redir(&cmd->redir, redir);
			tmp = tmp->next->next;
		}
		else
			tmp = tmp->next;
	}
	cmd->cmd = collect_cmd_args(start, tmp);
	return (tmp);
}

t_cmd	*making_cmd(t_token *token)
{
	t_cmd	*first;
	t_cmd	*last;
	t_cmd	*cmd;

	first = NULL;
	last = NULL;
	while (token)
	{
		cmd = init_cmd();
		if (!cmd)
			return (NULL);
		token = fill_cmd(token, cmd);
		if (!first)
			first = cmd;
		else
			last->next = cmd;
		last = cmd;
		if (token && token->type == PIPE)
			token = token->next;
	}
	return (first);
}
