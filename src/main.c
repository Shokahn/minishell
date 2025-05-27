/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:11:24 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/27 16:59:21 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_shell(t_data *shell)
{
	shell->input = NULL;
	shell->sep = 0;
	shell->line = NULL;
	shell->token = NULL;
	shell->cmd = NULL;
	shell->env = NULL;
	shell->store = NULL;
}

int	minishell(char *input, t_data *shell)
{
	shell->input = input;
	if (!lexeur(shell))
		return (0);
	if (!making_token(shell))
		return (0);
	if (!shell->env)
		return (0);
	if (!expandation(shell))
		return (0);
	if (!parsing(shell))
		return (0);
	shell->cmd = making_cmd(shell->token);
	setup_exec(shell);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_data	shell;

	(void)av;
	if (ac != 1)
	{
		ft_putstr_fd(ITALIC RED UNDERLINE "Error do not put argument\n" RESET,
			2);
		exit(1);
	}
	init_shell(&shell);
	setup_signals();
	shell.env = get_env(&shell, envp);
	while (1)
	{
		input = readline(GREEN BOLD "minishell> " RESET);
		if (!input)
			break ;
		else
		{
			add_history(input);
			if (!minishell(input, &shell))
				continue ;
		}
	}
}

// FINIR BUILTIN EXPORT