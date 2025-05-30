/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_recuting.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:29:59 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/30 16:19:01 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	*fill_the_tab(int *tab, char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] && input[i] == '"')
			i = pass_the_quote(input, i + 1, '"');
		else if (input[i] && input[i] == '\'')
			i = pass_the_quote(input, i + 1, '\'');
		else if (input[i] && ft_isspace(input[i]))
		{
			tab[i] = 1;
			i++;
		}
		else
			i++;
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
	ft_free_str(&(current->inside));
	current->inside = ft_strdup(line[i++]);
	current->expand = 0;
	current->next = NULL;
	tmp = current;
	while (line[i])
	{
		new = malloc(sizeof(t_token));
		if (!new)
			return (NULL);
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
	line = malloc(sizeof(char *) * (count + 1));
	if (!line)
		return (ft_free_int(&tab), NULL);
	line = makesplit(line, tab, current->inside);
	if (!line)
		return ft_free_int(&tab), (NULL);
	ft_free_int(&tab);
	current = divide_the_expanded_token(line, current);
	if (!current)
		return (ft_free_tab(&line), NULL);
	current->type = 0;
	ft_free_tab(&(line));
	return (current);
}

int	expand_token_recuting(t_data *shell)
{
	t_token	*current;
	t_token	*tmp;

	current = shell->token;
	while (current)
	{
		if (current->expand > 0)
		{
			current = token_cuting(current);
			if (!current)
				return (0);
			tmp = current;
			while (tmp)
			{
				tmp = tmp->next;
			}
		}
		else
			current = current->next;
	}
	return (1);
}
