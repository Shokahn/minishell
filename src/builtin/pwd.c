/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:29:32 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/30 17:02:41 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd(void)
{
	char	wd[PATH_MAX];

	if (getcwd(wd, sizeof(wd)) != NULL)
		printf("%s\n", wd);
	else
		perror("getcwd error");
}
