/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:57:08 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/30 16:57:33 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_token(t_token **token)
{
	t_token	*current;
	t_token	*tmp;

	if (!token | !*token)
		return ;
	current = *token;
	while (current)
	{
		tmp = current->next;
		ft_free_str(&(current->inside));
		free(current);
		current = tmp;
	}
	*token = NULL;
}

void	free_redir(t_redir **redir)
{
	t_redir	*current;
	t_redir	*tmp;

	if (!redir || !*redir)
		return ;
	current = *redir;
	while (current)
	{
		tmp = current->next;
		ft_free_str(&(current->file));
		free(current);
		current = tmp;
	}
	*redir = NULL;
}

void	free_cmd(t_cmd **cmd)
{
	t_cmd	*current;
	t_cmd	*tmp;

	if (!cmd | !*cmd)
		return ;
	current = *cmd;
	while (current)
	{
		tmp = current->next;
		ft_free_tab(&(current->cmd));
		free_redir(&(current->redir));
		free(current);
		current = tmp;
	}
	*cmd = NULL;
}

void	free_store(t_store **store)
{
	if (!store || !*store)
		return ;
	ft_free_tab(&((*store)->env_tab));
	free(*store);
	*store = NULL;
}

void	ft_free_data(t_data *shell)
{
	if (!shell)
		return ;
	ft_free_int(&(shell->sep));
	ft_free_str(&(shell->input));
	ft_free_tab(&(shell->line));
	free_token(&(shell->token));
	free_cmd(&(shell->cmd));
	free_store(&(shell->store));
}
