/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:26:20 by stdevis           #+#    #+#             */
/*   Updated: 2025/04/14 15:19:51 by stdevis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/headers/libft.h"
#include <dirent.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termcap.h>
#include <termios.h>
#include <ttyent.h>
#include <unistd.h>

#define TEST printf(BOLD RED "test\n" RESET);
#define TEST2 printf(BOLD GREEN "test\n" RESET);


typedef struct s_token
{
	int				type;
	char			*inside;
	struct s_token	*next;
	struct s_token	*prev;

}					t_token;

typedef struct s_data
{
	t_token			*token;

	char			*input;
	int				*sep;
	int				token_count;
	char			**line;
}					t_data;

// error
void				ft_error(t_data *shell, int type, char *message);
