
#include "../includes/minishell.h"

void	init_shell(t_data *shell)
{
	shell->input = NULL;
	shell->sep = 0;
	shell->line = NULL;
	shell->token = NULL;
}
int	pass_the_quote_separator(t_data *shell, char *input, char c, int i)
{
	i++;
	while (input[i] && input[i] != c)
		i++;
	if (!input[i])
		shell->sep[0] = 4;
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
	while (input[i])
	{
		count++;
		while (input[i] && (shell->sep[i] == 0 || shell->sep[i] == 1))
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
		while (input[i] && (shell->sep[i] == 0 || shell->sep[i] == 4))
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
	line = malloc(sizeof(char *) * (wcount(input, shell) + 2));
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
	return (1);
}

int	parsing(t_data *shell)
{
	if (shell->sep[0] == 4)
	{
		ft_error(shell, 0, "minishell : syntax error quote in unquoted cell\n");
		return (0);
	}
	return (1);
}

t_token	*create_node(char *inside)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->inside = ft_strdup(inside);
	if (!new->inside) // need to free still
	{
		return (NULL);
	}
	new->next = NULL;
	new->prev = NULL;
	new->type = 0;
	return (new);
}

t_token	*line_to_token(t_data *shell)
{
	int		i;
	t_token	*head;
	t_token	*tail;
	t_token	*new;

	i = 0;
	head = NULL;
	tail = NULL;
	while (shell->line[i])
	{
		new = create_node(shell->line[i]);
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		else
		{
			tail->next = new;
			new->prev = tail;
		}
		tail = new;
		i++;
	}
	return (head);
}

void	define_type(t_token *current)
{
	if (!ft_strncmp(current->inside, "|", 1))
		current->type = PIPE;
	else if (!ft_strncmp(current->inside, ">>", 2))
		current->type = APPEND;
	else if (!ft_strncmp(current->inside, "<<", 2))
		current->type = HEREDOC;
	else if (!ft_strncmp(current->inside, ">", 1))
		current->type = REDIR_OUT;
	else if (!ft_strncmp(current->inside, "<", 1))
		current->type = REDIR_IN;
	else
		current->type = WORD;
}

void	identification_token(t_data *shell)
{
	t_token	*current;

	current = shell->token;
	while (current)
	{
		define_type(current);
		current = current->next;
	}
}

int	making_the_list(t_data *shell)
{
	shell->token = line_to_token(shell);
	if (!shell->token)
		return (0);
	identification_token(shell);
	return (1);
}

void	print_token_list(t_data *shell)
{
	int		i;
	t_token	*current;

	i = 0;
	current = shell->token;
	while (current)
	{
		printf("[%d] : %s : type = %d\n", i, current->inside, current->type);
		current = current->next;
		i++;
	}
}

int	minishell(char *input, t_data *shell)
{
	shell->input = input;
	if (!ft_strncmp(input, "exit", 4)) // dont forget to free
	{
		printf("exit\n");
		exit(0);
	}
	if (!lexeur(shell))
		return (0);
	if (!parsing(shell))
		return (0);
	if (!making_the_list(shell))
		return (0);
	print_token_list(shell);
	return (1);
}

int	main(int ac, char **av)
{
	char	*input;
	t_data	shell;

	(void)av;
	if (ac != 1)
	{
		ft_putstr_fd(ITALIC RED UNDERLINE "Error do not put argument\n" RESET,
			2);
		exit(1);
	}
	init_shell(&shell);
	while (1)
	{
		input = readline(GREEN BOLD "minishell> " RESET);
		if (!input)
			break ;
		else
		{
			add_history(input);
			if (!minishell(input, &shell))
				continue ;
		}
		free(input);
	}
}
