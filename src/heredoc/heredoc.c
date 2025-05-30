/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:13:52 by brcoppie          #+#    #+#             */
/*   Updated: 2025/05/30 17:17:04 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_heredoc(t_cmd *cmd)
{
	t_redir	*redir;

	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir && redir->type == HEREDOC && redir->fd != -1)
			{
				close(redir->fd);
				unlink(redir->file);
				ft_free_str(&(redir->file));
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

static void	heredoc_child(int fd, char *delimiter, t_data *data)
{
	char	*input;

	signal(SIGINT, SIG_DFL);
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			write(2, "warning: here-document delimited", 33);
			write(2, " by end-of-file (wanted '", 26);
			write(2, delimiter, ft_strlen(delimiter));
			write(2, "')\n", 3);
			break ;
		}
		else if (ft_strcmp(delimiter, input) == 0)
		{
			free(input);
			break ;
		}
		input = expand_string_heredoc(input, data);
		(write(fd, input, ft_strlen(input)), write(fd, "\n", 1));
		free(input);
	}
	close(fd);
	exit(0);
}

static int	heredoc_parent(int fd, pid_t pid, char *tmp_file, t_data *data)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	close(fd);
	waitpid(pid, &status, 0);
	setup_sigint();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		unlink(tmp_file);
		ft_free_str(&tmp_file);
		data->exit_status = 130;
		return (0);
	}
	return (1);
}

int	exec_heredoc(char *delimiter, t_redir *redir, t_data *data)
{
	int		fd;
	char	*tmp_file;
	pid_t	pid;

	fd = create_temp_file(&tmp_file);
	if (fd == -1)
		return (0);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), close(fd), \
				unlink(tmp_file), ft_free_str(&tmp_file), 0);
	else if (pid == 0)
		heredoc_child(fd, delimiter, data);
	else
	{
		if (heredoc_parent(fd, pid, tmp_file, data) == 0)
			return (0);
	}
	ft_free_str(&(redir->file));
	redir->file = tmp_file;
	redir->fd = open(tmp_file, O_RDONLY);
	return (1);
}
