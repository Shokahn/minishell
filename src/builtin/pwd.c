/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:29:32 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/28 13:39:05 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd()
{
	char	wd[PATH_MAX];

	if (getcwd(wd, sizeof(wd)) != NULL)
		printf("%s\n", wd);
	else
		perror("getcwd error");
}
