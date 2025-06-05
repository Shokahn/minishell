/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 18:59:08 by brcoppie          #+#    #+#             */
/*   Updated: 2025/06/05 16:45:14 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t	g_sigint_catcher = 0;

void	sigint_heredoc_parent(int sig)
{
	(void)sig;
	g_sigint_catcher = 1;
}

void	sigint_heredoc_child(int sig)
{
	(void)sig;
	g_sigint_catcher = 1;
	write(1, "\n", 1);
	exit(130);
}

static void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_sigint_catcher = 1;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
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
