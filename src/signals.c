/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 18:59:08 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/30 16:37:14 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	pause_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	sig_handler(int sig)
{
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
