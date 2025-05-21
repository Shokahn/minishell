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
	char	*tmp;
	char	*join;
	int		i;

	i = 0;
	tab = malloc(sizeof(char *) * (ft_envsize(env) + 1));
	if (!tab)
		return (NULL);
	while (env)
	{
		tmp = ft_strjoin(env->name, "=");
		if (!tmp)
		{
			ft_free_tab(tab);
			return (NULL);
		}
		join = ft_strjoin(tmp, env->inside);
		free(tmp);
		if (!join)
		{
			ft_free_tab(tab);
			return (NULL);
		}
		tab[i] = join;
		if (!tab[i])
		{
			ft_free_tab(tab);
			return (NULL);
		}
		i++;
		env = env->next;
	}
	tab[i] = 0;
	return (tab);
}

char	**ft_tab_dup(char **tab)
{
	char	**copy;
	int		i;
	int		l;

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

char	**get_paths(char **env)
{
	int		i;
	char	**result;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp("PATH", env[i], 4) == 0)
		{
			result = ft_split(env[i], ':');
			result[0] = ft_substr(result[0], 5, ft_strlen(result[0]));
			return (result);
		}
		i++;
	}
	return (NULL);
}

char	*find_valid_path(const char *str, t_store *store)
{
	char	**paths;
	int		i;
	char	*pathname;
	char	*tmp;

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
	char	*path;

	if (access(cmd->cmd[0], X_OK) == 0)
		path = cmd->cmd[0];
	else
		path = find_valid_path(cmd->cmd[0], store);
    if (!path)
	{
		perror("command not found");
		exit(EXIT_FAILURE);
	}
    execve(path, cmd->cmd, store->env_tab);
    perror("execve failed");
	ft_free_tab(store->env_tab);
	free(store);
    exit(EXIT_FAILURE);
}

void	pickup_children(void)
{
	while (wait(NULL) > 0) // wait returns -1 when no children are left?
		;
	setup_signals();
}

int	open_pipe(int fd[2], t_cmd *current)
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
	dup2(fd, 0); // change stdin?
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
	dup2(fd, 1); // change stdout?
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
	dup2(fd, 1); // change stdout?
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
			break ;
		redir = redir->next;
	}
}

void ft_echo(char **args)
{
	int	i;
	int	newline_toggle;

	i = 1;
	newline_toggle = 1;
	if (strncmp(args[1], "-n", 3) == 0)
	{
		newline_toggle = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		i++;
	}
	if (newline_toggle == 1)
		ft_putchar_fd('\n', 1);
}

int	is_built_in(t_cmd *cmd)
{
	int		i;
	char	*built_in_funcs[7];

	if (!cmd || !cmd->cmd || !cmd->cmd[0])
		return (0);
	i = 0;
	built_in_funcs[0] = "echo";
	built_in_funcs[1] = "cd_2";
	built_in_funcs[2] = "pwd_2";
	built_in_funcs[3] = "export";
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

void	exec_built_in(t_store *store, t_data *data)
{
	(void)data;
	if (ft_strncmp(store->current->cmd[0], "echo_2", 7) == 0)
		ft_echo(store->current->cmd);
	if (ft_strncmp(store->current->cmd[0], "export", 7) == 0)
		builtin_export(store->current->cmd, data);
}

void    launch_child(t_store *store, t_data *data)
{
	setup_signals();
	if (store->in_fd != 0)
	{
		dup2(store->in_fd, 0); //if not first cmd, read from pipe
    	close(store->in_fd);
    }
    if (store->current->next)
    {
        close(store->fd[0]);
        dup2(store->fd[1], 1); //write in pipe
    	close(store->fd[1]);
    }
	if (store->current->redir)
		handle_redirections(store->current);
	if (is_built_in(store->current))
	{
		exec_built_in(store, data);
		exit(EXIT_SUCCESS);
	}
	else
		exec_cmd(store, store->current);
}

void    handle_parent(t_store *store)
{
	pause_signals();
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

void	save_fds(t_store *store)
{
	store->std_in = dup(0);
	store->std_out = dup(1);

	if (store->std_in < 0 || store->std_out < 0)
		perror("dup fail");
}

void	reset_fds(t_store *store)
{
	dup2(store->std_in, 0);
	dup2(store->std_out, 1);
	close(store->std_in);
	close(store->std_out);
}

void	exec_cmds(t_store *store, t_data *data)
{
	if (is_built_in(store->current) && !store->current->next)
	{
		if (store->current->redir)
		{
			save_fds(store);
			handle_redirections(store->current);
		}
		exec_built_in(store, data);
		if (store->current->redir)
			reset_fds(store);
		store->current = store->current->next;
	}
    while (store->current)
    {
			if (open_pipe(store->fd, store->current) == 0)
            	return ;
        	store->pid = fork();
        	if (store->pid == -1)
        	{
            	perror("fork");
            	return ;
        	}
        	else if (store->pid == 0)
            	launch_child(store, data);
        	else
            	handle_parent(store);
    }
    pickup_children();
}

void	setup_exec(t_data *data)
{
	data->store = malloc(sizeof(t_store));
    if (!data->store)
	{
        perror("Failed to allocate memory for store");
        return;
    }
	init_store(data->store, data);
	exec_cmds(data->store, data);
	ft_free_tab(data->store->env_tab);
	free(data->store);
}
