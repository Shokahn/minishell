/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 18:59:08 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/17 19:03:37 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sig_handler(int sig, siginfo_t *info, void *nothing)
{
	(void)nothing;
	(void)info;
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, BOLD"\n\033[32mminishell> \033[0m", 22);
	}
	else
		return ;
}

// sigaction is overkill
void	setup_signals(void)
{
	struct sigaction	sa;

	sa = (struct sigaction){0};
	sa.sa_sigaction = sig_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
// control+D sends EOF --> to handle in readline loop || automatic?
// free is not automatic!!
