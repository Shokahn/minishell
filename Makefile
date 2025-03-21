# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/04 15:05:03 by stdevis           #+#    #+#              #
#    Updated: 2025/03/21 17:52:41 by stdevis          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[36m
RESET = \033[0m
RED = \033[31m
BOLD = \033[1m
UNDERLINE = \033[4m
ITALIC = \033[3m

OBJ_DIR = obj/
SRC_DIR = src/

SRC = lexeur.c prompt.c
OBJ = $(addprefix $(OBJ_DIR), $(SRC:%.c=%.o))

CFLAGS = -Wall -Wextra -Werror -g3
FSANITIZE = -fsanitize=address
MAKE = make --no-print-directory

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

NAME = minishell

all: $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c 
	@mkdir -p $(OBJ_DIR)
	@echo "📦 $(ITALIC)$(YELLOW)Compiling $< $(RESET)"
	@cc $(CFLAGS) -c $< -o $@


$(NAME): $(LIBFT) $(OBJ)
	@echo ""
	@echo "		🚀 $(BOLD)$(YELLOW)Linking $(NAME)...$(RESET)"
	@cc $(CFLAGS) $(OBJ) -o $(NAME) -L $(LIBFT_DIR) -lft
	@echo ""
	@echo "	🎉 $(BOLD)$(GREEN)SUCCESS: $(NAME) has been created$(RESET) ✅ "
	@echo ""

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "		⚙ $(UNDERLINE)$(BOLD)Building $(NAME)$(RESET) ⚙"
	@echo ""

clean:
	@echo ""
	@echo "		🧹 $(BOLD)$(BLUE)Cleaning object files 🧹$(RESET)"
	@echo ""
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)
	@echo "💾 $(YELLOW)Cleaning $(NAME) object files$(RESET)"
	@echo " ↪️ $(YELLOW)$(BOLD)$(NAME) object files have been cleaned$(RESET) ✅"
	@echo ""
	@echo "	👉 $(BOLD)$(GREEN)Object files have been cleaned$(RESET) ❎"


fclean: clean
	@echo ""
	@echo "		🧹 $(BOLD)$(BLUE)Cleaning everything 🧹$(RESET)"
	@echo ""
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "💾 $(YELLOW)Cleaning $(NAME)$(RESET)"
	@echo " ↪️ $(YELLOW)$(BOLD)$(NAME) has been cleaned$(RESET) ✅"
	@rm -f $(NAME)
	@echo ""
	@echo "	👉 $(BOLD)$(GREEN)Everything has been cleaned$(RESET) ❎"
	@echo ""

re: fclean $(NAME)

.PHONY: all clean fclean re