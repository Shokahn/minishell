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

# define TEST printf(BOLD RED "test\n" RESET);
# define TEST2 printf(BOLD GREEN "test\n" RESET);

typedef enum e_type
{
	WORD,
	PIPE,
	REDIR_IN,  // <
	REDIR_OUT, // >
	APPEND,    // >>
	HEREDOC,   // <<
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
	t_token			*token;
	t_cmd			*cmd;
	t_env			*env;
	t_store			*store;
}					t_data;

/*typedef void (*built_in_func)(char **args);

typedef struct s_built_in_cmd
{
	const char		*cmd;
	built_in_func	handler;
	s_built_in_cmd	*next;
}	t_built_in_cmd;*/

// exec
void				setup_exec(t_data *data);

// error
void				ft_error(t_data *shell, int type, char *message);

// signals
void				setup_signals(void);
void				setup_sigint(void);
void				pause_signals(void);

// builtin
int					builtin_export(char **args, t_data *shell);

//heredoc
void				exec_heredoc(char *delimiter, t_cmd *cmd, t_data *data);

#endif