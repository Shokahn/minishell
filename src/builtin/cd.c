/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:25:54 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/31 18:26:11 by brcoppie         ###   ########.fr       */
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

static int	change_dir(t_data *data, char **paths)
{
	char	*home;
	char	*older_wd;

	home = check_value(data, "HOME");
	older_wd = check_value(data, "OLDPWD");
	if (!(paths[1]) || ft_strcmp(paths[1], "~") == 0)
	{
		if (chdir(home) != 0)
			return (free(home), free(older_wd), 0);
	}
	else if (ft_strcmp(paths[1], "-") == 0)
	{
		if (chdir(older_wd) != 0)
			return (free(home), free(older_wd), 0);
	}
	else if (chdir(paths[1]) != 0)
		return (free(home), free(older_wd), 0);
	return (free(home), free(older_wd), 1);
}

int	ft_cd(t_data *data, char **paths)
{
	char	wd[PATH_MAX];
	char	old_wd[PATH_MAX];

	if (ft_tab_len(paths) > 2)
		return (write(2, "cd: too many arguments\n", 24), 0);
	if (getcwd(old_wd, sizeof(old_wd)) == NULL)
		perror("getcwd error");
	if (!change_dir(data, paths))
		return (perror("chdir"), 0);
	if (getcwd(wd, sizeof(wd)) != NULL)
		update_env(&data->env, "PWD", wd, 0);
	else
		perror("getcwd error");
	update_env(&data->env, "OLDPWD", old_wd, 0);
	return (1);
}
