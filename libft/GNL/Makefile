# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/12 18:04:58 by stdevis           #+#    #+#              #
#    Updated: 2025/03/21 18:04:21 by stdevis          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_FILES = get_next_line get_next_line_utils

OBJ_DIR = obj/
SRC_DIR = src/

CFLAGS = -Wall -Wextra -Werror
OBJF = .cache_exists
NAME = GNL.a

GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[36m
RESET = \033[0m

BOLD = \033[1m
UNDERLINE = \033[4m
ITALIC = \033[3m

OBJ = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))
SRC = $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))


all: $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(OBJF)
	@cc $(CFLAGS) -c $< -o $@

$(OBJF):
	@mkdir -p $(OBJ_DIR)
	@touch $(OBJF)

$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_DIR_BONUS)
	@rm -rf $(OBJF)
		
fclean: clean
	@rm -f $(NAME)
	
re: fclean all

.PHONY:
	re make all fclean clean
