/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_variable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:30:55 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/31 17:29:08 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	handle_special_var(int i, t_token *current, t_data *shell)
{
	char	*expand;

	if (g_sigint_catcher != 0)
	{
		shell->exit_status = 130;
		g_sigint_catcher = 0;
	}
	expand = ft_itoa(shell->exit_status);
	if (!replace_value(expand, current, i, i + 1))
		return (ft_free_str(&expand), ft_exit(shell, NULL, "1"), 0);
	return (ft_free_str(&expand), i);
}

int	handle_alpha_var(char *inside, int i, t_token *current, t_data *shell)
{
	int		start;
	char	*name;
	char	*expand;

	start = i;
	i = end_of_expansion_or_not(inside, i + 1);
	name = ft_substr(inside, start, i - start);
	expand = check_value(shell, name);
	if (!replace_value(expand, current, start, i))
		return (ft_free_str(&name), ft_free_str(&expand), ft_exit(shell, NULL,
				"1"), 0);
	return (ft_free_str(&name), ft_free_str(&expand), start - 1);
}

int	handle_digit_var(int i, t_token *current, t_data *shell)
{
	char	*expand;

	expand = ft_strdup("");
	if (!replace_value(expand, current, i, i + 1))
		return (ft_free_str(&expand), ft_exit(shell, NULL, "1"), 0);
	return (ft_free_str(&expand), i - 1);
}

int	extract_variable(char *inside, int i, t_token *current, t_data *shell)
{
	current->expand++;
	if (inside[i] == '?')
		return (handle_special_var(i, current, shell));
	else if (ft_isalpha(inside[i]) || inside[i] == '_')
		return (handle_alpha_var(inside, i, current, shell));
	else if (ft_isdigit(inside[i]))
		return (handle_digit_var(i, current, shell));
	return (i + 1);
}
