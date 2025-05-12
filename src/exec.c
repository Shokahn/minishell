/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:21:15 by bri               #+#    #+#             */
/*   Updated: 2025/05/12 17:58:12 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_envsize(t_env *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

char	**ft_list_to_tab(t_env *env)
{
	char	**tab;
	int		i;

	i = 0;
	tab = malloc(sizeof(char *) * (ft_envsize(env) + 1));
	if (!tab)
		return (NULL);
	while (env)
	{
		tab[i] = ft_strdup(env->str);
		if (!tab[i++])
		{
			perror("free the rest of tab to avoid leaks, too lazy to do it right now");
			return (NULL);
		}
		env = env->next;
	}
	tab[i] = 0;
	return (tab);
}

char    **ft_tab_dup(char **tab)
{
    char    **copy;
    int     i;
    int     l;

    if (!tab || !tab[0])
        return (NULL);
    l = 0;
    while (tab[l])
        l++;
    copy = malloc(sizeof(char *) * (l + 1));
    i = 0;
    while (tab[i])
    {
        copy[i] = ft_strdup(tab[i]);
        if (!copy[i])
        {
            ft_free_tab(copy);
            return (NULL);
        }
        i++;
    }
    copy[i] = 0;
    return (copy);
}

char    **get_paths(char **env)
{
    int     i;

    i = 0;
    while (env[i])
    {
        if (ft_strncmp("PATH", env[i], 4) == 0)
            return (ft_split(env[i], ':'));
        i++;
    }
    return (NULL);
}

char    *find_valid_path(const char *str, t_store *store)
{
    char    **paths;
    int     i;
    char    *pathname;
    char    *tmp;

    paths = get_paths(store->env_tab);
    if (!paths)
        return (NULL);
    i = 0;
    while (paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        pathname = ft_strjoin(tmp, str);
        if (access(pathname, X_OK) == 0)
        {
            ft_free_tab(paths);
            return (pathname);
        }
        free(tmp);
        free(pathname);
        i++;
    }
    ft_free_tab(paths);
    return (NULL);
}

void    exec_cmd(t_store *store, t_cmd *cmd)
{
    char    *path;

    path = find_valid_path(cmd->cmd[0], store);
    if (!path)
	{
		perror("command not found");
    	exit(EXIT_FAILURE);
	}
    execve(path, cmd->cmd, store->env_tab);
    perror("execve failed");
    exit(EXIT_FAILURE);
}

void    pickup_children(void)
{
    while (wait(NULL) > 0) //wait returns -1 when no children are left?
        ;
}

int open_pipe(int fd[2], t_cmd *current)
{
    if (current->next)
    {
        if (pipe(fd) == -1)
        {
            perror("pipe");
            return (0);
        }
    }
    return (1);
}

void	redir_in_handler(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, 0); //change stdin?
	close(fd);
}

void	redir_out_handler(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, 1);  //change stdout?
	close(fd);
}

void	append_handler(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, 1);  //change stdout?
	close(fd);
}

void	handle_redirections(t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			redir_in_handler(redir);
		else if (redir->type == REDIR_OUT)
			redir_out_handler(redir);
		else if (redir->type == APPEND)
			append_handler(redir);
		else
			break;
		redir = redir->next;
	}
}

void    launch_child(t_store *store)
{
    if (store->in_fd != 0)
    {
        dup2(store->in_fd, 0); //if not first cmd, read from pipe
        close(store->in_fd);
    }
	if (store->current->redir)
		handle_redirections(store->current);
    if (store->current->next)
    {
        close(store->fd[0]);
        dup2(store->fd[1], 1); //write in pipe
        close(store->fd[1]);
    }
    exec_cmd(store, store->current);
}

void    handle_parent(t_store *store)
{
    if (store->in_fd != 0)
        close(store->in_fd);
    if (store->current->next)
    {
        close(store->fd[1]);
        store->in_fd = store->fd[0]; //save for next
    }
    store->current = store->current->next;
}

void	init_store(t_store *store, t_data *data)
{
	store->pid = -2;
	store->fd[0] = -2;
	store->fd[1] = -2;
	store->in_fd = 0;
	store->current = data->cmd;
	store->env_tab = ft_list_to_tab(data->env);
}

void ft_echo(char **args, int fd_out)
{
	int	i;
	int	newline_toggle;

	if (fd_out == -2)
		fd_out = 1;
	i = 1;
	newline_toggle = 1;
	if (strncmp(args[1], "-n", 3) == 0)
	{
		newline_toggle = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], fd_out);
		i++;
	}
	if (newline_toggle == 1)
		ft_putchar_fd('\n', fd_out);
}

int	is_built_in(t_cmd *cmd)
{
	int		i;
	char	*built_in_funcs[7];

	i = 0;
	built_in_funcs[0] = "echo_2";
	built_in_funcs[1] = "cd_2";
	built_in_funcs[2] = "pwd_2";
	built_in_funcs[3] = "export_2";
	built_in_funcs[4] = "unset_2";
	built_in_funcs[5] = "env_2";
	built_in_funcs[6] = "exit_2";
	while (i <= 6)
	{
		if (ft_strncmp(cmd->cmd[0], built_in_funcs[i], 10) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	exec_built_in(t_store *store)
{
	if (ft_strncmp(store->current->cmd[0], "echo_2", 7) == 0)
		ft_echo(store->current->cmd, store->fd[1]);
}

//split this function
void    setup_exec(t_data *data)
{
	t_store	*store;

	store = malloc(sizeof(t_store));
    if (!store)
	{
        perror("Failed to allocate memory for store");
        return;
    }
	init_store(store, data);
    while (store->current)
    {
		if (is_built_in(store->current))
		{
			exec_built_in(store);
			store->current = store->current->next;
		}
		else
		{
			if (open_pipe(store->fd, store->current) == 0)
            	return ;
        	store->pid = fork();
        	if (store->pid == -1)
        	{
            	perror("fork");
            	return ;
        	}
        	else if (store->pid == 1)
            	launch_child(store);
        	else
            	handle_parent(store);
		}
    }
    pickup_children();
	free(store);
}
