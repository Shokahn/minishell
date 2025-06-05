/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:29:32 by brcoppie          #+#    #+#             */
/*   Updated: 2025/06/05 16:57:08 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_pwd(void)
{
	char	wd[PATH_MAX];

	if (getcwd(wd, sizeof(wd)) != NULL)
		return (printf("%s\n", wd), 1);
	else
		return (perror("getcwd error"), 1);
}
