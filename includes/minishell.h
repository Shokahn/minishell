/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brcoppie <brcoppie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:07:53 by stdevis           #+#    #+#             */
/*   Updated: 2025/05/31 17:57:27 by brcoppie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/headers/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termcap.h>
# include <termios.h>
# include <ttyent.h>
# include <unistd.h>

# define PATH_MAX 4096

typedef enum e_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
}					t_type;

typedef struct s_token
{
	t_type			type;
	char			*inside;
	int				expand;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_redir
{
	t_type			type;
	char			*file;
	int				fd;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			**cmd;
	t_redir			*redir;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_env
{
	char			*inside;
	char			*name;
	struct s_env	*next;
}					t_env;

typedef struct s_store
{
	int				fd[2];
	int				in_fd;
	pid_t			pid;
	t_cmd			*current;
	char			**env_tab;
	int				std_in;
	int				std_out;
}					t_store;

typedef struct s_data
{
	char			*input;
	int				*sep;
	char			**line;
	int				exit_status;
	t_token			*token;
	t_cmd			*cmd;
	t_env			*env;
	t_store			*store;
}					t_data;

typedef struct s_split_ij
{
	int				j;
	int				i;
}					t_split_ij;

extern volatile sig_atomic_t	g_sigint_catcher;

// lexeur
int				lexeur(t_data *shell);

// split
char			**makesplit(char **line, int *sep, char *input);
int				wcount(char *input, int *sep);

// token
int				making_token(t_data *shell);

// expand
int				expandation(t_data *shell);
int				extract_variable(char *inside, int i, t_token *current,
					t_data *shell);
int				expand_token_recuting(t_data *shell);
char			*check_value(t_data *shell, char *name);
int				replace_value(char *expand, t_token *current, int start,
					int end);
int				end_of_expansion_or_not(char *inside, int i);
char			*check_value(t_data *shell, char *name);

// parsing
int				parsing(t_data *shell);
int				token_cleaning(t_data *shell);
char			*remove_quote(t_token *current);
int				pass_the_quote(char *inside, int i, char c);

// cmd
t_cmd			*making_cmd(t_token *token);
char			**collect_cmd_args(t_token *start, t_token *end);

// env
t_env			*get_env(t_data *shell, char **envp);
int				ft_envsize(t_env *lst);
char			**ft_list_to_tab(t_env *env);

// exec
void			setup_exec(t_data *data);
void			handle_redirections(t_cmd *cmd, t_data *data);
char			*find_valid_path(char *str, t_store *store);
int				open_pipe(int fd[2], t_cmd *current);
void			exec_cmd(t_store *store, t_cmd *cmd, t_data *data);
void			launch_child(t_store *store, t_data *data);
void			handle_parent(t_store *store);

// error
void			ft_error(int type, char *message);

// signals
void			setup_signals(void);
void			setup_sigint(void);
void			pause_signals(void);

// builtin
void			save_fds(t_store *store);
void			reset_fds(t_store *store);
int				is_built_in(t_cmd *cmd);
void			exec_built_in(t_store *store, t_data *data);
int				builtin_export(char **args, t_data *shell);
void			builtin_unset(char **cmd, t_data *shell);
void			print_env(t_data *shell);
void			ft_echo(char **args);
int				print_export(t_env *env);
void			ft_exit(t_data *data, t_cmd *cmd, char *force_status);
int				ft_cd(t_data *data, char **paths);
void			ft_pwd(void);

// env
char			**ft_list_to_tab(t_env *env);
int				ft_envsize(t_env *lst);
void			print_env(t_data *shell);
void			update_env(t_env **env_list, char *name, char *value, int append);

// heredoc
void			check_for_heredoc(t_cmd *cmd);
int				create_temp_file(char **temp_file);
int				exec_heredoc(char *delimiter, t_redir *redir, t_data *data);
void			close_heredoc(t_cmd *cmd);
void			init_heredoc(t_data *data);
char			*expand_string_heredoc(char *inside, t_data *shell);

// print
void			print_token(t_data *shell);
void			print_line(t_data *shell);
void			print_cmds(t_cmd *cmd);
void			print_sep(int *sep, char *input);

// free

void			ft_free_data(t_data *shell);
void			free_env(t_env **env);

#endif