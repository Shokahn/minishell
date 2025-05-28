/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:25:54 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/28 13:58:31 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_cd(t_data *data, char *path)
{
	char	wd[PATH_MAX];
	char	old_wd[PATH_MAX];

	if (getcwd(old_wd, sizeof(old_wd)) != NULL)
		perror("getcwd error");
	if (chdir(path) != 0)
	{
		perror("chdir");
		return ;
	}
	if (getcwd(wd, sizeof(wd)) != NULL)
		update_env(&data->env, "PWD", wd);
	else
		perror("getcwd error");
	update_env(&data->env, "OLDPWD", old_wd);
}
