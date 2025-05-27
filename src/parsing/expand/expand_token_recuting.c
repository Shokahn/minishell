/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_recuting.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:29:59 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/27 19:42:44 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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
	if (current->inside)
		free(current->inside);
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
