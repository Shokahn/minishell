/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:13:52 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/26 20:04:27 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* //use softer version of expand
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
	free(before);
	after = ft_substr(current->inside, i, ft_strlen(current->inside) - i);
	joined = ft_strjoin(tmp, after);
	free(tmp);
	free(after);
	free(current->inside);
	current->inside = joined;
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
		expand = check_value(shell, name);
		replace_value(expand, current, start, i);
	}
	else if (ft_isdigit(inside[i]))
	{
		expand = ft_strdup("");
		replace_value(expand, current, i, i + 1);
	}
	return (i + 1);
}

static int	expand_string(t_token *current, t_data *shell)
{
	int		i;
	int		check;
	int		i;
	char	suffix[2];

	i = 0;
	check = 0;
	while (current->inside[i])
	{
		if (current->inside[i] == '$' && current->inside[i + 1])
			i = extract_variable(current->inside, i + 1, current, shell);
		else
			i++;
	}
	return (1);
} */
static int	create_temp_file(char **temp_file)
{
	i = 0;
	suffix[1] = '\0';
	while (i < 10)
	{
		suffix[0] = i + '0';
		*temp_file = ft_strjoin(".heredoc_", suffix);
		if (!(*temp_file))
			return (-1);
		if (access(*temp_file, F_OK) != 0)
		{
			return (open(*temp_file, O_CREAT | O_RDWR | O_APPEND, 0600));
		}
		else
			free(*temp_file);
		i++;
	}
	return (-1);
}

void	exec_heredoc(char *delimiter, t_cmd *cmd, t_data *data)
{
	int		fd;
	char	*input;
	char	*tmp_file;

	(void)data;
	pause_signals();
	fd = create_temp_file(&tmp_file);
	if (fd == -1)
		return ;
	while (1)
	{
		input = readline("> ");
		if (!input || (ft_strcmp(delimiter, input) == 0))
		{
			free(input);
			printf("\n");
			break ;
		}
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	close(fd);
	cmd->redir->file = tmp_file;
	cmd->redir->fd = open(tmp_file, O_RDONLY);
	setup_signals();
}
