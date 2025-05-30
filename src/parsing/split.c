/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:38:16 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/30 23:56:39 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_word_count(char *input, int *sep, int *i)
{
	if (sep[*i] == 0)
	{
		while (input[*i] && sep[*i] == 0)
			(*i)++;
		return (1);
	}
	else if (sep[*i] == 2)
	{
		(*i)++;
		return (1);
	}
	else if (sep[*i] == 3)
	{
		(*i) += 2;
		return (1);
	}
	else if (sep[*i] == 1)
		while (input[*i] && sep[*i] == 1)
			(*i)++;
	else
		(*i)++;
	return (0);
}

int	wcount(char *input, int *sep)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input[i])
		count += handle_word_count(input, sep, &i);
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

int	handle_special_sep(char **line, char *input, t_split_ij *s, int *sep)
{
	if (sep[s->i] == 2)
	{
		line[(s->j)++] = wcreate(s->i, s->i + 1, input);
		(s->i)++;
		return (1);
	}
	if (sep[s->i] == 3)
	{
		line[(s->j)++] = wcreate(s->i, s->i + 2, input);
		(s->i) += 2;
		return (1);
	}
	return (0);
}

char	**makesplit(char **line, int *sep, char *input)
{
	t_split_ij	s;
	int			start;

	s.i = 0;
	s.j = 0;
	while (input[s.i])
	{
		while (input[s.i] && sep[s.i] == 1)
			s.i++;
		start = s.i;
		if (handle_special_sep(line, input, &s, sep))
			continue ;
		while (input[s.i] && (sep[s.i] == 0 || sep[s.i] == 4))
			s.i++;
		if (start < s.i)
		{
			line[s.j] = wcreate(start, s.i, input);
			if (!line[s.j++])
				return (ft_free_index((&line), s.j - 1), NULL);
		}
	}
	line[s.j] = NULL;
	return (line);
}
