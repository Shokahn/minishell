/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:21:15 by bri               #+#    #+#             */
/*   Updated: 2025/05/03 21:08:07 by brcoppie         ###   ########.fr       */
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

char    *find_valid_path(const char *str, t_data *data)
{
    char    **paths;
    int     i;
    char    *pathname;
    char    *tmp;

    paths = get_paths(data->env_tab);
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

void    exec_cmd(t_data **data, t_cmd **cmd)
{
    char    *path;

    path = find_valid_path((*cmd)->cmd[0], *data);
    if (!path)
	{
		perror("command not found");
    	exit(EXIT_FAILURE);
	}
    execve(path, (*cmd)->cmd, (*data)->env_tab);
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

//Heredoc = do nothing right now, to implement!!!
//implementing heredoc : 
// https://medium.com/@oduwoledare/heredoc-a-deep-dive-23c82992e522 interesting article
// shell inside of shell, have to redo parsing on input, shellception??? POG 
void	ft_heredoc(t_redir *redir)
{
	if (redir->type == HEREDOC)
		write(1, "no heredoc yet\n", 15);
	return ;
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
		else if (redir->type == HEREDOC)
			ft_heredoc(redir);
		else
			break;
		redir = redir->next;
	}
}

void    launch_child(int *in_fd, t_cmd **current, t_data **data, int *fd)
{
    if (*in_fd != 0)
    {
        dup2(*in_fd, 0); //if not first cmd, read from pipe
        close(*in_fd);
    }
	if ((*current)->redir)
		handle_redirections(*current);
    if ((*current)->next)
    {
        close(fd[0]);
        dup2(fd[1], 1); //write in pipe
        close(fd[1]);
    }
    exec_cmd(data, current);
}

void    handle_parent(int *in_fd, t_cmd **current, int *fd)
{
    if (*in_fd != 0)
        close(*in_fd);
    if ((*current)->next)
    {
        close(fd[1]);
        *in_fd = fd[0]; //save for next
    }
    *current = (*current)->next;
}

//split this function
void    setup_exec(t_data *data)
{
    int     fd[2];
    int     in_fd;
    pid_t   pid;
    t_cmd   *current;

    in_fd = 0; //stdin
	data->env_tab = ft_list_to_tab(data->env);
    current = data->cmd;
    while (current)
    {
        if (open_pipe(fd, current) == 0)
            return ;
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return ;
        }
        else if (pid == 0)
            launch_child(&in_fd, &current, &data, fd);
        else
            handle_parent(&in_fd, &current, fd);
    }
    pickup_children();
}
