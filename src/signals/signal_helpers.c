/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:47:38 by brcoppie          #+#    #+#             */
/*   Updated: 2025/06/05 13:49:19 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	child_signal(void)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
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
