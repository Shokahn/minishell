/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:58:06 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/28 15:13:54 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_token(t_data *shell)
{
	t_token	*current;
	int		i;

	i = 0;
	current = shell->token;
	while (current)
	{
		printf("token[%d] = %s\n", i, current->inside);
		current = current->next;
		i++;
	}
	printf(BOLD GREEN "----------\n" RESET);
}

void	print_line(t_data *shell)
{
	int	i;

	i = 0;
	printf(BOLD CYAN "----------\n" RESET);
	while (shell->line[i])
	{
		printf("line[%d] = %s\n", i, shell->line[i]);
		i++;
	}
	printf(BOLD RED "----------\n" RESET);
}

void	print_cmds(t_cmd *cmd)
{
	t_redir	*r;
	int		y;
	int		i;

	y = 1;
	while (cmd)
	{
		printf("CMD[%d]:\n", y);
		i = 0;
		while (cmd->cmd && cmd->cmd[i])
		{
			printf("  arg[%d]: %s\n", i, cmd->cmd[i]);
			i++;
		}
		r = cmd->redir;
		while (r)
		{
    		printf("  redir: type = %d, file = %s\n", r->type, r->file);
			r = r->next;
		}
		printf("----------\n");
		cmd = cmd->next;
		y++;
	}
}

void	print_sep(t_data *shell)
{
	int	i;

	i = 0;
	while (shell->input[i])
	{
		printf("[%d]", shell->sep[i]);
		i++;
	}
	printf("\n");
	printf(BOLD YELLOW "----------\n" RESET);
}
