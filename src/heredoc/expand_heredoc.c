/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shokahn <shokahn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:48:24 by stdevis           #+#    #+#             */
/*   Updated: 2025/06/01 21:55:41 by shokahn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	replace_value_heredoc(char **inside, char *expand, int start, int end)
{
	char	*before;
	char	*after;
	char	*tmp;
	char	*joined;

	if (start <= 0)
		before = ft_strdup("");
	else
		before = ft_strndup(*inside, start - 1);
	tmp = ft_strjoin(before, expand);
	after = ft_substr(*inside, end, ft_strlen(*inside) - end);
	joined = ft_strjoin(tmp, after);
	ft_free_str(&before);
	ft_free_str(&tmp);
	ft_free_str(&after);
	ft_free_str(inside);
	if (!joined)
		return (0);
	*inside = joined;
	return (1);
}

int	handle_special_var_heredoc(char **inside, int i, t_data *shell)
{
	char	*expand;

	if (g_sigint_catcher != 0)
	{
		shell->exit_status = 130;
		g_sigint_catcher = 0;
	}
	expand = ft_itoa(shell->exit_status);
	if (!replace_value_heredoc(inside, expand, i, i + 1))
		return (ft_free_str(&expand), ft_exit(shell, NULL, "1"), 0);
	return (ft_free_str(&expand), i);
}

int	handle_alpha_var_heredoc(char **inside, int i, t_data *shell)
{
	int		start;
	char	*name;
	char	*expand;

	start = i;
	i = end_of_expansion_or_not(*inside, i + 1);
	name = ft_substr(*inside, start, i - start);
	expand = check_value(shell, name);
	if (!replace_value_heredoc(inside, expand, start, i))
		return (ft_free_str(&name), ft_free_str(&expand), ft_exit(shell, NULL,
				"1"), 0);
	return (ft_free_str(&name), ft_free_str(&expand), start - 1);
}

int	handle_digit_var_heredoc(char **inside, int i, t_data *shell)
{
	char	*expand;

	expand = ft_strdup("");
	if (!replace_value_heredoc(inside, expand, i, i + 1))
		return (ft_free_str(&expand), ft_exit(shell, NULL, "1"), 0);
	return (ft_free_str(&expand), i - 1);
}

int	extract_variable_heredoc(char **inside, int i, t_data *shell)
{
	if ((*inside)[i] == '?')
		return (handle_special_var_heredoc(inside, i, shell));
	else if (ft_isalpha((*inside)[i]) || (*inside)[i] == '_')
		return (handle_alpha_var_heredoc(inside, i, shell));
	else if (ft_isdigit((*inside)[i]))
		return (handle_digit_var_heredoc(inside, i, shell));
	return (i + 1);
}
