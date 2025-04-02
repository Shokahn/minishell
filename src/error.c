
#include "../includes/minishell.h"

void	free_error(t_data *shell, int type, char *message)
{
	if (shell)
	{
		if (shell->first)
			free_node(shell->first);
		free(shell);
	} 
	if (type == 0)
		ft_putstr_fd(message, 2);
	else if (type == 1)
		perror(message);
	exit(1);
}