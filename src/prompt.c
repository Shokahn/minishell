
#include "../includes/minishell.h"

void	init_shell(t_data *shell)
{
	shell->input = NULL;
	shell->sep = 0;
	shell->line = NULL;
	shell->token = NULL;
	shell->cmd = NULL;
	shell->env = NULL;
	shell->store = NULL;
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

int	wcount(char *input, int *sep)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input[i])
	{
		if (input[i] && sep[i] == 4)
			i++;
		if (input[i] && sep[i] == 0)
			count++;
		while (input[i] && sep[i] == 0)
			i++;
		while (input[i] && sep[i] == 1)
			i++;
		if (input[i] && sep[i] == 2)
		{
			count++;
			i++;
		}
		if (input[i] && sep[i] == 3)
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

char	**makesplit(char **line, int *sep, char *input)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (input[i])
	{
		while (input[i] && sep[i] == 1)
			i++;
		start = i;
		if (sep[i] == 2)
		{
			line[j++] = wcreate(i, i + 1, input);
			i++;
			continue ;
		}
		if (sep[i] == 3)
		{
			line[j++] = wcreate(i, i + 2, input);
			i += 2;
			continue ;
		}
		while (input[i] && (sep[i] == 0 || sep[i] == 4))
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
	count = wcount(input, shell->sep);
	line = malloc(sizeof(char *) * (count));
	if (!line)
		return (NULL);
	line = makesplit(line, shell->sep, shell->input);
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

int	pass_the_quote(char *inside, int i, char c)
{
	while (inside[i] && inside[i] != c)
		i++;
	return (i + 1);
}

char	*remove_quote(t_token *current)
{
	int		i;
	int		j;
	int		in_single;
	int		in_double;
	char	*result;

	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (current->inside[i])
		i++;
	result = malloc(sizeof(char) * (i + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (current->inside[i])
	{
		if (current->inside[i] == '\'' && in_double == 0)
			in_single = !in_single;
		else if (current->inside[i] == '\"' && in_single == 0)
			in_double = !in_double;
		else
			result[j++] = current->inside[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

void	token_cleaning(t_data *shell)
{
	t_token	*current;

	current = shell->token;
	while (current)
	{
		current->inside = remove_quote(current);
		current = current->next;
	}
}

int	*fill_the_tab(int *tab, char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!ft_isspace(input[i]))
			i++;
		else if (input[i] && input[i] == '"')
			i = pass_the_quote(input, i, '"');
		else if (input[i] && input[i] == '\'' && input[i])
			i = pass_the_quote(input, i, '\'');
		else if (input[i] && ft_isspace(input[i]))
		{
			tab[i] = 1;
			i++;
		}
	}
	return (tab);
}

t_token	*divide_the_expanded_token(char **line, t_token *current)
{
	int		i;
	t_token	*new;
	t_token	*last;
	t_token	*tmp;

	i = 0;
	if (!line || !*line)
		return (NULL);
	last = current->next;
	current->inside = ft_strdup(line[i++]);
	current->expand = 0;
	current->next = NULL;
	tmp = current;
	while (line[i])
	{
		new = malloc(sizeof(t_token));
		if (!new)
			return (NULL); // free
		new->inside = ft_strdup(line[i++]);
		new->expand = 0;
		new->type = 0;
		new->prev = tmp;
		new->next = NULL;
		tmp->next = new;
		tmp = new;
	}
	tmp->next = last;
	return (current);
}

t_token	*token_cuting(t_token *current)
{
	int		*tab;
	int		len;
	char	**line;
	int		count;

	len = ft_strlen(current->inside);
	tab = ft_calloc(len, 4);
	if (!tab)
		return (NULL);
	tab = fill_the_tab(tab, current->inside);
	count = wcount(current->inside, tab);
	line = malloc(sizeof(char *) * count);
	if (!line)
		return (NULL);
	line = makesplit(line, tab, current->inside);
	if (!line)
		return (NULL);
	free(tab);
	current = divide_the_expanded_token(line, current);
	if (!current)
		return (NULL);
	current->type = 0;
	return (current);
}

int	expand_token_cuting(t_data *shell)
{
	t_token	*current;
	t_token	*tmp;

	current = shell->token;
	while (current)
	{
		if (current->expand > 0)
		{
			current = token_cuting(current);
			tmp = current;
			while (tmp)
			{
				printf("current = %s | type = %d\n", tmp->inside, tmp->type);
				tmp = tmp->next;
			}
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
				"minishell: syntax error near unexpected token 'newline'\n"),
			0);
	token_cleaning(shell);
	if (!(expand_token_cuting(shell)))
		return (0);
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
	new->expand = 0;
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

int	making_token(t_data *shell)
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
			args[i] = ft_strdup(start->inside);
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
	char	*split[2];
	int		i;
	int		start;

	i = 0;
	new = malloc(sizeof(t_env));
	if (!new)
		return (ft_error(shell, 0, "malloc failed\n"), NULL);
	new->next = NULL;
	while (str[i] && str[i] != '=')
		i++;
	split[0] = ft_calloc(sizeof(char), (i + 2));
	split[0] = ft_substr(str, 0, i);
	start = i + 1;
	while (str[i])
		i++;
	split[1] = ft_calloc(sizeof(char), i - start + 1);
	split[1] = ft_substr(str, start, i - start);
	new->name = split[0];
	new->inside = split[1];
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
		printf("%s=", tmp->name);
		printf("%s\n", tmp->inside);
		tmp = tmp->next;
	}
}

int	end_of_expansion_or_not(char *inside, int i)
{
	while (inside[i] && (ft_isalpha(inside[i]) || ft_isdigit(inside[i])
			|| inside[i] == '_'))
		i++;
	return (i);
}

char	*check_value(t_data *shell, char *name)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (!ft_strcmp(name, current->name))
			return (current->inside);
		current = current->next;
	}
	return (ft_strdup(""));
}

void	replace_value(char *expand, t_token *current, int start, int i)
{
	char	*before;
	char	*after;
	char	*tmp;
	char	*joined;

	before = ft_strndup(current->inside, start - 1);
	tmp = ft_strjoin(before, expand);
	printf("before = %s\n", before);
	free(before);
	after = ft_substr(current->inside, i, ft_strlen(current->inside) - i);
	joined = ft_strjoin(tmp, after);
	printf("after = %s\n", after);
	free(tmp);
	free(after);
	free(current->inside);
	current->inside = joined;
	printf("current->inside = %s\n", current->inside);
}

int	extract_variable(char *inside, int i, t_token *current, t_data *shell)
{
	int		start;
	char	*name;
	char	*expand;

	current->expand++;
	if (ft_isalpha(inside[i]) || inside[i] == '_')
	{
		start = i;
		i = end_of_expansion_or_not(inside, i + 1);
		name = ft_substr(inside, start, i - start);
		printf(GREEN "i after expand = %d\n" RESET, i);
		printf(RED "name = %s\n" RESET, name);
		expand = check_value(shell, name);
		printf(RED "expand = %s\n" RESET, expand);
		replace_value(expand, current, start, i);
	}
	else if (ft_isdigit(inside[i]))
	{
		expand = ft_strdup("");
		replace_value(expand, current, i, i + 1);
	}
	return (i + 1);
}

int	expand_string(t_token *current, t_data *shell)
{
	int	i;
	int	in_the_dquote;

	i = 0;
	in_the_dquote = 0;
	while (current->inside[i])
	{
		if (current->inside[i] == '\'' && current->inside[i + 1] && in_the_dquote == 0)
			i = pass_the_quote(current->inside, i + 1, '\'');
		else if (current->inside[i] == '\"' && current->inside[i + 1])
		{	
			in_the_dquote = !in_the_dquote;
			i++;
		}
		else if (current->inside[i] == '$' && current->inside[i + 1]
			&& current->inside[i + 1] != '$')
			i = extract_variable(current->inside, i + 1, current, shell);
		else
			i++;
	}
	return (1);
}

int	expandation(t_data *shell)
{
	t_token	*tmp;

	tmp = shell->token;
	while (tmp)
	{
		if (tmp->type == WORD)
			if (!expand_string(tmp, shell))
				return (0);
		tmp = tmp->next;
	}
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
	if (!making_token(shell))
		return (0);
	shell->env = get_env(shell, envp);
	// print_env(shell);
	if (!shell->env)
		return (0);
	print_token(shell);
	if (!expandation(shell))
		return (0);
	if (!parsing(shell))
		return (0);
	shell->cmd = parse_tokens(shell->token);
	// print_line(shell);
	print_token(shell);
	print_cmds(shell->cmd);
	printf("\n\n\033[1mOutput :\033[0m\n\n");
	setup_exec(shell);
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
	setup_signals();
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

// FINIR BUILTIN EXPORT + EXPAND QUI RECOMMENCE PAS !