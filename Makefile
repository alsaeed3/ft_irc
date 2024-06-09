# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alsaeed <alsaeed@student.42abudhabi.ae>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/05 23:41:54 by alsaeed           #+#    #+#              #
#    Updated: 2024/06/07 00:01:51 by alsaeed          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# colors #
GREEN 		= \033[32m
RED 		= \033[31m
BOLD_GREEN 	= \033[1;32m
BOLD_RED 	= \033[1;31m
RESET 		= \033[0m

NAME = ircserv

CXX = c++

CXXFLAGS = -Wall -Werror -Wextra -std=c++98

SRCS =	Server.cpp

OBJS_DIR = object_files

OBJS = $(SRCS:%.cpp=$(OBJS_DIR)/%.o)

$(OBJS_DIR)/%.o: sources%.cpp
	@mkdir -p $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS) main.cpp
	@$(CXX) $(CXXFLAGS) $(OBJS) main.cpp -o $@
	@echo "ircserv compiled: $(BOLD_GREEN)OK$(RESET)"

$(OBJS):	Makefile \
			main.cpp \
			$(SRCS)

clean:
	@if [ -e $(OBJS_DIR) ]; then \
		rm -rf $(OBJS_DIR); \
		echo "ircserv clean: $(BOLD_RED)OK$(RESET)"; \
	fi

fclean: clean
	@if [ -e $(NAME) ]; then \
		rm -rf $(NAME); \
		echo "ircserv full-clean: $(BOLD_RED)OK$(RESET)"; \
	fi

re: fclean all

.PHONY: all clean fclean re