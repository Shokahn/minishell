/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shokahn <shokahn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:57:44 by stdevis           #+#    #+#             */
/*   Updated: 2025/06/08 20:19:33 by shokahn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

long	check_range(const char *str, long sign, unsigned long result, int i)
{
	unsigned long	limit;
	unsigned long	tmp;

	if (sign == -1)
		limit = (unsigned long)LONG_MAX + 1;
	else
		limit = (unsigned long)LONG_MAX;
	while (str[i] >= '0' && str[i] <= '9')
	{
		tmp = result;
		result *= 10;
		result += str[i] - 48;
		if (result < tmp)
		{
			errno = ERANGE;
			return (0);
		}
		i++;
	}
	if (result > limit)
	{
		errno = ERANGE;
		return (0);
	}
	return (sign * result);
}

long	ft_atol(const char *str)
{
	unsigned long	result;
	long			sign;
	int				i;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	return (check_range(str, sign, result, i));
}

int	all_empty_arg(char **arg)
{
	int i;

	i = 1;
	while(arg[i])
	{
		if (arg[i][0])
			return (0);
		i++;
	}
	return (1);
}