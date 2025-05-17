#include "../includes/minishell.h"

void	sig_handler(int signal, siginfo_t *info, void *nothing)
{
	(void)nothing;
	(void)info;
	if (signal == SIGINT)
		write(1,  BOLD"\n\033[32mminishell> \033[0m" RESET, 22);
	else
		return ;
}

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
