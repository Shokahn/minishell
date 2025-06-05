/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 18:59:08 by brcoppie          #+#    #+#             */
/*   Updated: 2025/06/05 13:04:20 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t	g_sigint_catcher = 0;

void	child_signal(void)
{
	signal(SIGQUIT, SIG_DFL);
	setup_sigint();
}

static void	sigint_catcher(int sig)
{
	(void)sig;
	g_sigint_catcher = 1;
	printf("\n");
}

void	sigquit_handler(int sig)
{
	(void)sig;
	ft_putstr_fd("Quit (core dumped)\n", 2);
}

void	pause_signals(void)
{
	signal(SIGINT, sigint_catcher);
	signal(SIGQUIT, sigquit_handler);
}

static void	sig_handler(int sig)
{
	g_sigint_catcher = 1;
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		write(1, "\n\033[1m\033[32mminishell> \033[0m", 26);
	}
}

void	setup_sigint(void)
{
	signal(SIGINT, sig_handler);
}

// sigaction is overkill --> use signal
void	setup_signals(void)
{
	setup_sigint();
	signal(SIGQUIT, SIG_IGN);
}
// control+D sends EOF
