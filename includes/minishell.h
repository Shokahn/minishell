/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:26:20 by stdevis           #+#    #+#             */
/*   Updated: 2025/04/22 14:16:20 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/headers/libft.h"
# include <dirent.h>
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
	HEREDOC    // <<

}					t_type;

typedef struct s_token
{
	t_type			type;
	char			*inside;
	struct s_token	*next;
	struct s_token	*prev;

}					t_token;

typedef struct s_data
{
	char			*input;
	int				*sep;
	char			**line;
	t_token			*token;

}					t_data;

// error
void				ft_error(t_data *shell, int type, char *message);

#endif