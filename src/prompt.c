
#include "../includes/minishell.h"


















int	main(void)
{
	char	*cwd;
	char	*input;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd failed\n");
		return (1);
	}
	while (1)
	{
		input = readline(GREEN BOLD "minishell> " RESET);
		if (!input)
			break ;
		else
		{	
			add_history(input);
			minishell(input);
		}
		free(input);
	}
	free(cwd);
}
