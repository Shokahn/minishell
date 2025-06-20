/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:11:24 by stdevis           #+#    #+#             */
/*   Updated: 2025/06/09 11:09:35 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_shell(t_data *shell, char **envp)
{
	setup_signals();
	shell->input = NULL;
	shell->sep = 0;
	shell->line = NULL;
	shell->token = NULL;
	shell->cmd = NULL;
	shell->env = NULL;
	shell->store = NULL;
	shell->exit_status = 0;
	shell->fork_check = 0;
	shell->in_the_quote = 0;
	shell->env = get_env(shell, envp);
}

int	minishell(char *input, t_data *shell)
{
	shell->input = input;
	if (!lexeur(shell))
		return (ft_free_data(shell), 0);
	if (!making_token(shell))
		return (ft_free_data(shell), 0);
	if (!expandation(shell))
		return (ft_free_data(shell), 0);
	if (!parsing(shell))
		return (ft_free_data(shell), 0);
	shell->cmd = making_cmd(shell->token);
	if (!shell->cmd)
		return (ft_free_data(shell), 0);
	g_sigint_catcher = 0;
	setup_exec(shell);
	return (ft_free_data(shell), 1);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_data	shell;

	if (ac != 1 && av)
	{
		ft_putstr_fd(ITALIC RED "Error do not put argument\n" RESET, 2);
		exit(1);
	}
	init_shell(&shell, envp);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		else
		{
			if (input[0])
				add_history(input);
			if (!minishell(input, &shell))
				continue ;
		}
	}
	free_env(&(shell.env));
	ft_putstr_fd("exit\n", 2);
}
