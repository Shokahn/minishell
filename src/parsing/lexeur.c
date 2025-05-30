/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexeur.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:25:27 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/30 13:37:21 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	while (input && input[i])
	{
		if (input[i] != '|' && !(ft_isspace(input[i])) && input[i] != '"'
			&& input[i] != '\'' && input[i] != '<' && input[i] != '>')
			i++;
		else if (input[i] == '"')
			i = pass_the_quote_separator(shell, input, '"', i);
		else if (input[i] == '\'')
			i = pass_the_quote_separator(shell, input, '\'', i);
		else if (ft_isspace(input[i]))
			shell->sep[i++] = 1;
		else if (((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<'
					&& input[i + 1] == '<')))
		{
			shell->sep[i] = 3;
			i += 2;
		}
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			shell->sep[i++] = 2;
	}
}

char	**split_line(t_data *shell, char *input)
{
	char	**line;
	int		count;

	define_separator(shell, input);
	count = wcount(input, shell->sep);
	line = malloc(sizeof(char *) * (count + 1));
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
	shell->sep = ft_calloc(sizeof(int), (len_input + 1));
	if (!shell->sep)
		return (ft_error(shell, 0, "sep malloc failed\n"), 0);
	if (!validate_syntax(shell, shell->input))
		return (0);
	shell->line = split_line(shell, shell->input);
	if (!shell->line)
		return (ft_error(shell, 0, "split line failed\n"), 0);
	return (1);
}
