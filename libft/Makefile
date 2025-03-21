# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: stdevis <stdevis@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/21 15:03:53 by stdevis           #+#    #+#              #
#    Updated: 2025/03/21 18:30:00 by stdevis          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[36m
RESET = \033[0m
BOLD = \033[1m
UNDERLINE = \033[4m

LIBFT_DIR = libft_original
PRINTF_DIR = Printf
GNL_DIR = GNL

LIBFT = $(LIBFT_DIR)/libft_original.a
PRINTF = $(PRINTF_DIR)/libftprintf.a
GNL = $(GNL_DIR)/GNL.a

NAME = libft.a 
MAKE = make --no-print-directory

all: $(NAME)

$(NAME):
	@echo ""
	@echo "		⚙ $(UNDERLINE)$(BOLD)Building libft$(RESET) ⚙"
	@$(MAKE) -C $(LIBFT_DIR)
	@$(MAKE) -C $(PRINTF_DIR)
	@$(MAKE) -C $(GNL_DIR)
	@echo ""
	@ar rcs $(NAME) $(LIBFT) $(PRINTF) $(GNL)
	@ranlib $(NAME)
	@echo "    📚 $(BOLD)$(BLUE)SUCCESS: $(NAME) has been created$(RESET) 📚"
	@echo ""

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(MAKE) -C $(PRINTF_DIR) clean
	@$(MAKE) -C $(GNL_DIR) clean

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(MAKE) -C $(PRINTF_DIR) fclean
	@$(MAKE) -C $(GNL_DIR) fclean
	@rm -f $(NAME)
	@echo "💾 $(YELLOW)Cleaning $(NAME)$(RESET)"
	@echo "↪️ $(BOLD)$(YELLOW)libft has been cleaned$(RESET) ✅"

re: fclean all

.PHONY: all clean fclean re