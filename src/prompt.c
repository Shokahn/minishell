
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
	shell->len_input = 0;
	shell->line = NULL;
	shell->count_sep = 0;
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
		free_error(shell, 0, "No quote ending the first one\n");
	return (i + 1);
}

void	count_separator(t_data *shell, char *input)
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
			shell->count_sep++;
			i++;
		}
		else if (((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<'
					&& input[i + 1] == '<')))
		{
			shell->sep[i] = 3;
			shell->sep[i + 1] = -1;
			shell->count_sep++;
			i += 2;
		}
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			shell->sep[i] = 2;
			shell->count_sep++;
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
	while (input[i])
	{
		while (input[i] && shell->sep[i] == 1)
			i++;
		if (input[i] && shell->sep[i] != 1)
		{
			count++;
			i++;
			while (input[i] && shell->sep[i] != 1 && shell->sep[i] != 2)
				i++;
		}
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

void	validate_syntax(t_data *shell, char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if ((input[i] == '>' || input[i] == '<') && input[i + 1] == input[i]
			&& input[i + 2] == input[i])
			free_error(shell, 0, "3 or more redirection sign consecutively\n");
		if (input[i] == '|' && input[i + 1] == '|')
			free_error(shell, 0, "2 or more pipes consecutively\n");
		i++;
	}
}

char	**split_line(t_data *shell, char *input)
{
	char	**line;

	validate_syntax(shell, input);
	count_separator(shell, input);
	line = malloc(sizeof(char *) * (wcount(input, shell) + 1));
	if (!line)
		return (NULL);
	line = makesplit(line, shell, shell->input);
	if (!line)
		return (NULL);
	return (line);
}

void	lexeur(t_data *shell)
{
	int	i;

	i = 0;
	shell->len_input = ft_strlen(shell->input);
	shell->sep = ft_calloc(sizeof(int), shell->len_input);
	if (!shell->sep)
		free_error(shell, 0, "sep malloc failed\n");
	shell->line = split_line(shell, shell->input);
	if (!shell->line[i])
		free_error(shell, 0, "split line failed\n");
	/* 	while (shell->line[i])
		{
			phrase_identification(shell->line[i], i);
			i++;
		} */
	while (shell->line[i])
	{
		printf("[%d] : %s\n", i, shell->line[i]);
		i++;
	}
}
void	minishell(char *input, t_data *shell)
{
	shell->input = input;
	if (!ft_strncmp(input, "exit", 4)) //dont forget to free
		exit(0);
	if (!shell)
		free_error(shell, 0, "Shell malloc allocation failed\n");
	lexeur(shell);
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
			minishell(input, shell);
		}
		free(input);
	}
}
