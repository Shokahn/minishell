/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:25:54 by brcoppie          #+#    #+#             */
/*   Updated: 2025/06/09 11:11:15 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_tab_len(char **tab)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (tab[i])
	{
		if (tab[i][0])
			count++;
		i++;
	}
	return (count);
}

static int	change_dir(t_data *data, char **paths)
{
	char	*home;
	char	*older_wd;

	home = check_value(data, "HOME");
	older_wd = check_value(data, "OLDPWD");
	if (all_empty_arg(paths) || !(paths[1]) || ft_strcmp(paths[1], "~") == 0)
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
		return (write(2, "cd: too many arguments\n", 24), 1);
	else if (getcwd(old_wd, sizeof(old_wd)) == NULL)
		perror("getcwd error: old directory");
	if (!change_dir(data, paths))
		return (perror("chdir"), 1);
	else if (getcwd(wd, sizeof(wd)) != NULL)
		update_env(&data->env, "PWD", wd, 0);
	else
		return (perror("getcwd error"), update_env(&data->env, "OLDPWD", old_wd,
				0), 1);
	update_env(&data->env, "OLDPWD", old_wd, 0);
	return (0);
}
