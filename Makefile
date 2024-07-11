# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shamzaou@student.42abudhabi.ae <shamzaou>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/10 19:44:25 by shamzaou@student  #+#    #+#              #
#    Updated: 2024/07/10 19:44:25 by shamzaou@student ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 #-g3 -fsanitize=address
INCLUDES = -Iincludes/ -Ibonus

SRC_DIR = source_files
BONUS_DIR = bonus
OBJS_DIR = object_files

SRCS =	$(addprefix $(SRC_DIR)/, \
		Server.cpp \
		Channel.cpp \
		Client.cpp \
		ParseMessage.cpp \
		nickCommand.cpp \
		quitCommand.cpp \
		joinCommand.cpp \
		privateMessage.cpp \
		Commands.cpp \
		modeCommand.cpp \
		inviteCommand.cpp \
		kickCommand.cpp \
		motdCommand.cpp \
		noticeCommand.cpp \
		partCommand.cpp \
		topicCommand.cpp) \
		$(addprefix $(BONUS_DIR)/, \
		ProfanityPatrol.cpp)

OBJS = $(SRCS:%.cpp=$(OBJS_DIR)/%.o)

# colors #
GREEN 		= \033[0;32m
RED 		= \033[0;31m
YELLOW 		= \033[0;33m
BLUE 		= \033[0;34m
BOLD_GREEN 	= \033[1;32m
BOLD_RED 	= \033[1;31m
BOLD_YELLOW = \033[1;33m
BOLD_BLUE 	= \033[1;34m
RESET 		= \033[0m

all: $(NAME)

$(OBJS_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "$(BOLD_YELLOW)compiling$(RESET) $(GREEN)$<$(RESET):\r\t\t\t\t\t"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $(INCLUDES) $< -o $@
	@echo "\r\t\t\t\t\t$(RED)$(CXX) $(CXXFLAGS)$(RESET)$(BLUE)-c $< -o $@$(RESET) $(BOLD_GREEN)<OK>$(RESET)"

$(OBJS_DIR)/$(BONUS_DIR)/%.o: $(BONUS_DIR)/%.cpp
	@echo "$(BOLD_YELLOW)compiling$(RESET) $(GREEN)$<$(RESET):\r\t\t\t\t\t"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $(INCLUDES) $< -o $@
	@echo "\r\t\t\t\t\t$(RED)$(CXX) $(CXXFLAGS)$(RESET)$(BLUE)-c $< -o $@$(RESET) $(BOLD_GREEN)<OK>$(RESET)"

$(NAME): $(OBJS) main.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) main.cpp -o $@
	@echo "$(BOLD_YELLOW)ircserv Compiled$(RESET): $(BOLD_GREEN)<OK>$(RESET)"

clean:
	@rm -rf $(OBJS_DIR)
	@echo "$(BOLD_YELLOW)ircserv Clean$(RESET): $(BOLD_GREEN)<OK>$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(BOLD_YELLOW)ircserv Full-Clean$(RESET): $(BOLD_GREEN)<OK>$(RESET)"

re: fclean all

.PHONY: all clean fclean re