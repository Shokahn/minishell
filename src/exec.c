/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bri <bri@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:21:15 by bri               #+#    #+#             */
/*   Updated: 2025/05/02 13:17:54 by bri              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    free_tab(char **tab)
{
    int i;

    i = 0;
    if (!tab)
        return ;
    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
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

    paths = get_paths(data->env);
    if (!paths)
        return (NULL);
    i = 0;
    while (paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        pathname = ft_strjoin(tmp, str);
        if (access(pathname, X_OK) == 0)
        {
            free_tab(paths);
            return (pathname);
        }
        free(tmp);
        free(pathname);
        i++;
    }
    free_tab(paths);
    return (NULL);
}

void    exec_cmd(t_data **data, t_cmd **cmd)
{
    char    *path;

    path = find_valid_path((*cmd)->cmd[0], *data);
    if (!path)
        return ;
    execve(path, (*cmd)->cmd, (*data)->env);
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

void    launch_child(int in_fd, t_cmd **current, t_data **data, int *fd)
{
    if (in_fd != 0)
    {
        dup2(in_fd, 0); //if not first cmd, read from pipe
        close(in_fd);
    }
    if ((*current)->next)
    {
        close(fd[0]);
        dup2(fd[1], 1); //write in pipe
        close(fd[1]);
    }
    exec_cmd(data, current);
}

void    handle_parent(int in_fd, t_cmd **current, int *fd)
{
    if (in_fd != 0)
        close(in_fd);
    if ((*current)->next)
    {
        close(fd[1]);
        in_fd = fd[0]; //save for next
    }
    *current = (*current)->next;
}

void    setup_exec(t_data *data)
{
    int     fd[2];
    int     in_fd;
    pid_t   pid;
    t_cmd   *current;

    if (!data || !data->cmd)
        return ;
    in_fd = 0; //stdin
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
            launch_child(in_fd, &current, &data, fd);
        else
            handle_parent(in_fd, &current, fd);
    }
    pickup_children();
}

//
//
//

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
            free_tab(copy);
            return (NULL);
        }
        i++;
    }
    copy[i] = 0;
    return (copy);
}

int main(int ac, char **av, char **env)
{
    t_data  *data;
    t_cmd   *cmd;
    char    *cmd1[] = {"ls", "-l", NULL};
    char    *cmd2[] = {"grep", "a.out", NULL};

    (void)ac;
    (void)av;
    data = malloc(sizeof(t_data));
    cmd = malloc(sizeof(t_cmd));
    cmd->next = malloc(sizeof(t_cmd));
    data->env = ft_tab_dup(env);
    data->cmd = cmd;
    cmd->cmd = cmd1;
    cmd->next->cmd = cmd2;
    setup_exec(data);
    free(data);
    free(cmd->next);
    free(cmd);
}