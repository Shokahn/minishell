/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:09:26 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/28 17:49:41 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_error(t_data *shell, int type, char *message)
{
	(void)shell;
	if (type == 0)
	{
		ft_putstr_fd(ITALIC RED BOLD "Error : " RESET, 2);
		ft_putstr_fd(message, 2);
	}
	else if (type == 1)
		perror(message);
}

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

void	free_env(t_env **env)
{
	t_env	*current;
	t_env	*tmp;

	if (!env | !*env)
		return ;
	current = *env;
	while (current)
	{
		tmp = current->next;
		ft_free_str(&(current->inside));
		ft_free_str(&(current->name));
		free(current);
		current = tmp;
	}
	*env = NULL;
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
