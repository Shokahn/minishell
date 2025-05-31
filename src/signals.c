/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 18:59:08 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/31 17:54:41 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_sigint_catcher = 0;

void	pause_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

static void	sig_handler(int sig)
{
	g_sigint_catcher = 1;
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
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
