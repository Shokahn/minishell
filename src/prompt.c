
#include "../includes/minishell.h"

t_data	*init_shell(void)
{
	t_data	*shell;

	shell = malloc(sizeof(t_data));
	if (!shell)
		return (NULL);
	shell->first = NULL;
	shell->input = NULL;
	shell->sep = 0;
	shell->len = 0;
	shell->line = NULL;
	return (shell);
}

void	phrase_identification(char *node, int i)
{
	int	y;

	if (ft_strncmp(node, "|", ft_strlen(node)))
}

int	pass_the_quote_separator(char *input, char c, int i)
{
	i++;
	while (input[i] != c)
		i++;
	return (i);
}

void count_separator(t_data *shell, char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] != '|' && input[i] != ' ' && input[i] != '"'
			&& input[i] != '\'')
			i++;
		else if (input[i] == '"')
			i = pass_the_quote_separator(input, '"', i);
		else if (input[i] == '\'')
			i = pass_the_quote_separator(input, '\'', i);
		else if (input[i] == ' ')
		{
			shell->sep[i] = 1;
			shell->count_sep++;
			i++;
		}
		else if (input[i] == '|')
		{
			shell->sep[i] = 2;
			shell->count_sep++;
			i++;
		}
	}
}

char **makesplit(char **line, t_data *shell, char *input)
{
	int i;
	int j;
	int start;

	i = 0;
	j = 0;
	while(input[i])
	{
		while((shell->sep[i] == 2 || shell->sep[i] == 1) && i < shell->len)
		start = i;
		while((shell->sep[i] != 2 || shell->sep[i] != 1) && i < shell->len)
			i++
	}
	
}

char	**split_line(t_data *shell, char *input)
{
	int i;
	char **line;

	i = 0;
	count_separator(shell, input);
	line = malloc(sizeof(char *) * (shell->count_sep + 2));
	if (!line)
		return (NULL);
	line = makesplit(line, shell, shell->input);
	if (!line)
		return (NULL);
}

void	lexeur(t_data *shell)
{
	int	i;

	shell->len = ft_strlen(shell->input);
	shell->sep = ft_calloc(sizeof(int), shell->len);
	if (!shell->sep)
		free_error(shell, 0, "sep malloc failed\n");
	shell->line = split_line(shell, shell->input);
	if (!shell->line)
		free_error(shell, 0, "split line failed\n");
	i = 0;
	while (shell->line[i])
	{
		phrase_identification(shell->line[i], i);
		i++;
	}
}
void	minishell(char *input)
{
	t_data	*shell;

	shell = init_shell();
	shell->input = input;
	if (!shell)
		free_error(shell, 0, RED BOLD "Shell malloc allocation failed\n" RESET);
	lexeur(shell);
}

int	main(int ac, char **av)
{
	char	*input;

	(void)av;
	if (ac != 1)
	{
		ft_putstr_fd(ITALIC RED UNDERLINE "Error do not put argument\n" RESET,
			2);
		exit(1);
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
}
