/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:09:26 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/27 19:16:38 by stdevis          ###   ########.fr       */
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

void	free_token(t_token *token)
{
	t_token	*current;

	while (token)
	{
		current = token->next;
		if (token->inside)
			free(token->inside);
		free(token);
		token = current;
	}
}

void	free_redir(t_redir *redir)
{
	t_redir	*current;

	while (redir)
	{
		current = redir->next;
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = current;
	}
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*current;

	while (cmd)
	{
		current = cmd->next;
		if (cmd->cmd)
			ft_free_tab(cmd->cmd);
		free_redir(cmd->redir);
		free(cmd);
		cmd = current;
	}
}

void	free_env(t_env *env)
{
	t_env	*current;

	while (env)
	{
		current = env->next;
		if (env->inside)
			free(env->inside);
		if (env->name)
			free(env->name);
		free(env);
		env = current;
	}
}

void	free_store(t_store *store)
{
	if (store->env_tab)
		ft_free_tab(store->env_tab);
	if (store)
		free(store);
}

void	ft_free_data(t_data *shell)
{
	if (!shell)
		return ;
	if (shell->sep)
		free(shell->sep);
	if (shell->input)
		free(shell->input);
	if (shell->line)
		ft_free_tab(shell->line);
	free_token(shell->token);
	free_cmd(shell->cmd);
	free_env(shell->env);
	free_store(shell->store);
}
