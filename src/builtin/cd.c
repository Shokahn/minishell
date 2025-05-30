/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:25:54 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/30 18:59:44 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_tab_len(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

int	ft_cd(t_data *data, char **paths)
{
	char	wd[PATH_MAX];
	char	old_wd[PATH_MAX];
	char	*home;

	if (ft_tab_len(paths) > 2)
		return (printf("cd: too many arguments\n"), 0);
	home = check_value(data, "HOME");
	if (getcwd(old_wd, sizeof(old_wd)) == NULL)
		perror("getcwd error");
	if (!(paths[1]) || ft_strcmp(paths[1], "~") == 0)
	{
		if (chdir(home) != 0)
			return (free(home), perror("chdir"), 0);
	}
	else if (chdir(paths[1]) != 0)
		return (free(home), perror("chdir"), 0);
	if (getcwd(wd, sizeof(wd)) != NULL)
		update_env(&data->env, "PWD", wd);
	else
		perror("getcwd error");
	update_env(&data->env, "OLDPWD", old_wd);
	return (free(home), 1);
}
