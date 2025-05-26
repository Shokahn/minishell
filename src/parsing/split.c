/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:38:16 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/26 20:04:16 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
				return (ft_free_index(line, j - 1), NULL);
		}
	}
	line[j] = NULL;
	return (line);
}
