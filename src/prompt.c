
#include "../includes/minishell.h"

void	init_shell(t_data *shell)
{
	shell->input = NULL;
	shell->sep = 0;
	shell->line = NULL;
	shell->token = NULL;
	shell->cmd = NULL;
	shell->env = NULL;
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
		if (input[i] != '|' && !(ft_isspace(input[i])) && input[i] != '"'
			&& input[i] != '\'' && input[i] != '<' && input[i] != '>')
			i++;
		else if (input[i] == '"')
			i = pass_the_quote_separator(shell, input, '"', i);
		else if (input[i] == '\'')
			i = pass_the_quote_separator(shell, input, '\'', i);
		else if (ft_isspace(input[i]))
		{
			shell->sep[i] = 1;
			i++;
		}
		else if (((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<'
					&& input[i + 1] == '<')))
		{
			shell->sep[i] = 3;
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
		if (input[i] && shell->sep[i] == 4)
			i++;
		if (input[i] && shell->sep[i] == 0)
			count++;
		while (input[i] && shell->sep[i] == 0)
			i++;
		while (input[i] && shell->sep[i] == 1)
			i++;
		if (input[i] && shell->sep[i] == 2)
		{
			count++;
			i++;
		}
		if (input[i] && shell->sep[i] == 3)
		{
			count++;
			i += 2;
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
		if (shell->sep[i] == 3)
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

void	print_sep(t_data *shell)
{
	int	i;

	i = 0;
	while (shell->input[i])
	{
		printf("[%d]", shell->sep[i]);
		i++;
	}
	printf("\n");
	printf(BOLD YELLOW "----------\n" RESET);
}

char	**split_line(t_data *shell, char *input)
{
	char	**line;
	int		count;

	define_separator(shell, input);
	print_sep(shell);
	count = wcount(input, shell);
	printf("word count = %d\n", count);
	line = malloc(sizeof(char *) * (count));
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

int	pipe_startend(t_data *shell)
{
	t_token	*current;
			int i;

	current = shell->token;
	if (current->type == 1)
		return (0);
	while (current)
	{
		if (current->next == NULL)
		{
			if (current->type == 1)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	no_file_after_redir(t_data *shell)
{
	t_token	*current;

	current = shell->token;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == APPEND || current->type == HEREDOC)
		{
			if (!current->next)
				return (0);
			else if (current->next->type == 0)
				current = current->next;
			else
				return (0);
		}
		else
			current = current->next;
	}
	return (1);
}

int	parsing(t_data *shell)
{
	if (shell->sep[0] == 4)
		return (ft_error(shell, 0,
				"minishell : syntax error quote in unquoted cell\n"), 0);
	if (!(pipe_startend(shell)))
		return (ft_error(shell, 0,
				"minishell : syntax error near unexpected token '|'\n"), 0);
	if (!(no_file_after_redir(shell)))
		return (ft_error(shell, 0,
				"bash: syntax error near unexpected token 'newline'\n"), 0);
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

void	print_cmds(t_cmd *cmd)
{
	t_redir	*r;
	int		y;

	y = 1;
	while (cmd)
	{
		printf("CMD[%d]:\n", y);
		for (int i = 0; cmd->cmd && cmd->cmd[i]; i++)
			printf("  arg[%d]: %s\n", i, cmd->cmd[i]);
		r = cmd->redir;
		while (r)
		{
			printf("  redir: type = %d, file = %s\n", r->type, r->file);
			r = r->next;
		}
		printf("----------\n");
		cmd = cmd->next;
		y++;
	}
}

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->next = NULL;
	cmd->redir = NULL;
	return (cmd);
}

t_redir	*init_redir(t_token *token)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir || !token || !token->next)
		return (NULL);
	redir->type = token->type;
	redir->file = strdup(token->next->inside);
	redir->next = NULL;
	return (redir);
}

void	add_redir(t_redir **redir_list, t_redir *new)
{
	t_redir	*tmp;

	if (!*redir_list)
		*redir_list = new;
	else
	{
		tmp = *redir_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

int	count_args(t_token *start, t_token *end)
{
	t_token	*tmp;
	int		count;

	tmp = start;
	count = 0;
	while (tmp && tmp != end)
	{
		if (tmp->type == WORD)
			count++;
		else if (tmp->type != WORD)
			count--;
		tmp = tmp->next;
	}
	return (count);
}

char	**collect_cmd_args(t_token *start, t_token *end)
{
	char	**args;
	int		count_arg;
	int		i;
	int		check;

	i = 0;
	check = 0;
	count_arg = count_args(start, end);
	printf("nbr of args = %d\n", count_arg);
	args = malloc(sizeof(char *) * count_arg + 1);
	if (!args)
		return (NULL);
	while (start && start != end)
	{
		if (start->type == WORD && (check == 0 || (start->prev
					&& start->prev->type != REDIR_IN
					&& start->prev->type != REDIR_OUT
					&& start->prev->type != APPEND
					&& start->prev->type != HEREDOC)))
		{
			args[i] = strdup(start->inside);
			i++;
		}
		start = start->next;
		check++;
	}
	args[i] = NULL;
	return (args);
}

t_token	*fill_cmd(t_token *token, t_cmd *cmd)
{
	t_token	*start;
	t_token	*tmp;
	t_redir	*redir;

	start = token;
	tmp = token;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == REDIR_IN || tmp->type == REDIR_OUT
			|| tmp->type == APPEND || tmp->type == HEREDOC)
		{
			redir = init_redir(tmp);
			add_redir(&cmd->redir, redir);
			tmp = tmp->next->next;
		}
		else
			tmp = tmp->next;
	}
	cmd->cmd = collect_cmd_args(start, tmp);
	return (tmp);
}

t_cmd	*parse_tokens(t_token *token)
{
	t_cmd	*first;
	t_cmd	*last;
	t_cmd	*cmd;

	first = NULL;
	last = NULL;
	while (token)
	{
		cmd = init_cmd();
		token = fill_cmd(token, cmd);
		if (!first)
			first = cmd;
		else
			last->next = cmd;
		last = cmd;
		if (token && token->type == PIPE)
			token = token->next;
	}
	return (first);
}
void	print_token(t_data *shell)
{
	t_token	*current;
	int		i;

	i = 0;
	current = shell->token;
	while (current)
	{
		printf("token[%d] = %s\n", i, current->inside);
		current = current->next;
		i++;
	}
	printf(BOLD GREEN "----------\n" RESET);
}

void	print_line(t_data *shell)
{
	int	i;

	i = 0;
	printf(BOLD CYAN "----------\n" RESET);
	while (shell->line[i])
	{
		printf("line[%d] = %s\n", i, shell->line[i]);
		i++;
	}
	printf(BOLD RED "----------\n" RESET);
}

t_env	*add_env_node(char *str, t_data *shell)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (ft_error(shell, 0, "malloc failed\n"), NULL);
	new->next = NULL;
	new->str = str;
	return (new);
}

void	link_env_node(t_env **first, t_env *new)
{
	t_env	*tmp;

	if (!(*first))
	{
		*first = new;
		return ;
	}
	tmp = *first;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_env	*get_env(t_data *shell, char **envp)
{
	int		i;
	t_env	*first;
	t_env	*current;

	i = 0;
	first = NULL;
	while (envp[i])
	{
		current = add_env_node(envp[i], shell);
		if (!current)
			return (0);
		link_env_node(&first, current);
		i++;
	}
	return (first);
}

void	print_env(t_data *shell)
{
	t_env	*tmp;

	tmp = shell->env;
	while (tmp)
	{
		printf("%s\n", tmp->str);
		tmp = tmp->next;
	}
}

void	extract_variable(char **inside)
{
	


}

void expand_string(char **inside, t_data *shell)
{
	int i;

	i = 0;
	while (*inside[i])
	{
		if (*inside[i] == '$')
		{
			extract_variable(inside);
		}
	}
}


int	expandation(t_data *shell, char **envp)
{
	shell->env = get_env(shell, envp);
	if (!shell->env)
		return (0);
	while (shell->token)
	{
		if (shell->token->type == WORD)
			expand_string(&shell->token->inside, shell)
	}
	print_env(shell);
	return (1);
}

int	minishell(char *input, t_data *shell, char **envp)
{
	shell->input = input;
	if (!ft_strncmp(input, "exit", 4)) // dont forget to free
	{
		printf("exit\n");
		exit(0);
	}
	if (!lexeur(shell))
		return (0);
	if (!making_the_list(shell))
		return (0);
	if (!expandation(shell, envp))
		return (0);
	if (!parsing(shell))
		return (0);
	shell->cmd = parse_tokens(shell->token);
	print_line(shell);
	print_token(shell);
	print_cmds(shell->cmd);
	return (1);
}

int	main(int ac, char **av, char **envp)
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
			if (!minishell(input, &shell, envp))
				continue ;
		}
	}
}
