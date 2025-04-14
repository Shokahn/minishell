
#include "../includes/minishell.h"

t_data	*init_shell(void)
{
	t_data	*shell;

	shell = malloc(sizeof(t_data));
	if (!shell)
		return (NULL);
	shell->token = NULL;
	shell->input = NULL;
	shell->sep = 0;
	shell->token_count = 0;
	shell->line = NULL;
	return (shell);
}
/*
void	phrase_identification(char *node, int i)
{
	int	y;

	if (ft_strncmp(node, "|", ft_strlen(node)))
} */
int	pass_the_quote_separator(t_data *shell, char *input, char c, int i)
{
	i++;
	while (input[i] && input[i] != c)
		i++;
	if (!input[i])
		ft_error(shell, 0, "No quote ending the first one\n");
	return (i + 1);
}

void	define_separator(t_data *shell, char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] != '|' && input[i] != ' ' && input[i] != '"'
			&& input[i] != '\'' && input[i] != '<' && input[i] != '>')
			i++;
		else if (input[i] == '"')
			i = pass_the_quote_separator(shell, input, '"', i);
		else if (input[i] == '\'')
			i = pass_the_quote_separator(shell, input, '\'', i);
		else if (input[i] == ' ')
		{
			shell->sep[i] = 1;
			i++;
		}
		else if (((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<'
					&& input[i + 1] == '<')))
		{
			shell->sep[i] = 3;
			shell->sep[i + 1] = -1;
			i += 2;
		}
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			shell->sep[i] = 2;
			i++;
		}
	}
}

int	wcount(char *input, t_data *shell)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while(input[i])
	{
		count++;
		while(input[i] && (shell->sep[i] == 0 || shell->sep[i] == 1))
			i++;
		if (input[i] && shell->sep[i] == 2)
			count++;
		if (input[i] && shell->sep[i] == 3 && shell->sep[i + 1] == -1)
		{
			count++;
			i += 2;
		}
		i++;
	}
	return (count);
}

char	*wcreate(int start, int end, const char *str)
{
	char	*word;
	int		j;

	j = 0;
	word = malloc(sizeof(char) * ((end - start) + 1));
	if (!word)
		return (NULL);
	while (start < end)
	{
		word[j] = str[start];
		start++;
		j++;
	}
	word[j] = '\0';
	return (word);
}

char	**makesplit(char **line, t_data *shell, char *input)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (input[i])
	{
		while (input[i] && shell->sep[i] == 1)
			i++;
		start = i;
		if (shell->sep[i] == 2)
		{
			line[j++] = wcreate(i, i + 1, input);
			i++;
			continue ;
		}
		if (shell->sep[i] == 3 && shell->sep[i + 1] == -1)
		{
			line[j++] = wcreate(i, i + 2, input);
			i += 2;
			continue ;
		}
		while (input[i] && shell->sep[i] == 0)
			i++;
		if (start < i)
		{
			line[j] = wcreate(start, i, input);
			if (!line[j++])
			{
				ft_free_index(line, j - 1);
				return (NULL);
			}
		}
	}
	line[j] = NULL;
	return (line);
}

int	validate_syntax(t_data *shell, char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if ((input[i] == '>' || input[i] == '<') && input[i + 1] == input[i]
			&& input[i + 2] == input[i] && input[i + 3] == input[i])
			return (ft_error(shell, 0,
					"minishell: syntax error near unexpected token '>>'\n"), 0);
		if ((input[i] == '>' || input[i] == '<') && input[i + 1] == input[i]
			&& input[i + 2] == input[i])
			return (ft_error(shell, 0,
					"minishell: syntax error near unexpected token '>'\n"), 0);
		if (input[i] == '|' && input[i + 1] == '|')
			return (ft_error(shell, 0,
					"minishell: syntax error near unexpected token '|'\n"), 0);
		i++;
	}
	return (1);
}

char	**split_line(t_data *shell, char *input)
{
	char	**line;

	define_separator(shell, input);
	line = malloc(sizeof(char *) * ( wcount(input, shell) + 2));
	if (!line)
		return (NULL);
	line = makesplit(line, shell, shell->input);
	if (!line)
		return (NULL);
	return (line);
}

int	lexeur(t_data *shell)
{
	int	len_input;

	len_input = ft_strlen(shell->input);
	shell->sep = ft_calloc(sizeof(int), len_input);
	if (!shell->sep)
		return (ft_error(shell, 0, "sep malloc failed\n"), 0);
	if (!validate_syntax(shell, shell->input))
		return (0);
	shell->line = split_line(shell, shell->input);
	if (!shell->line)
		return (ft_error(shell, 0, "split line failed\n"), 0);
	while (shell->line[shell->token_count])
	{
		printf("[%d] : %s\n", shell->token_count,
			shell->line[shell->token_count]);
		shell->token_count++;
	}
	printf("%d\n", shell->token_count);
	return (1);
}
int	minishell(char *input, t_data *shell)
{
	shell->input = input;
	shell->token_count = 0;
	if (!ft_strncmp(input, "exit", 4)) // dont forget to free
	{
		printf("exit\n");
		exit(0);
	}
	if (!shell)
	{
		ft_error(shell, 0, "Shell malloc allocation failed\n");
		return (0);
	}
	if (!lexeur(shell))
		return (0);
	return (1);
}

int	main(int ac, char **av)
{
	char	*input;
	t_data	*shell;

	(void)av;
	if (ac != 1)
	{
		ft_putstr_fd(ITALIC RED UNDERLINE "Error do not put argument\n" RESET,
			2);
		exit(1);
	}
	shell = init_shell();
	while (1)
	{
		input = readline(GREEN BOLD "minishell> " RESET);
		if (!input)
			break ;
		else
		{
			add_history(input);
			if (!minishell(input, shell))
				continue ;
		}
		free(input);
	}
}
