# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/01 01:25:21 by vvazzs            #+#    #+#              #
#    Updated: 2026/06/17 08:22:05 by vvazzs           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98

NAME = webserver
PARSER_DIR = ./src/parser
SERVER_DIR = ./src/server
OBJ_DIR = obj

SRC =	src/main.cpp \
		$(PARSER_DIR)/parse.cpp \
		$(PARSER_DIR)/http_request.cpp \
		$(PARSER_DIR)/debugger.cpp \
		$(PARSER_DIR)/conf/conf.cpp \
		$(PARSER_DIR)/conf/conf_validation.cpp \
		$(PARSER_DIR)/conf/helpers.cpp \
		$(SERVER_DIR)/sSetup.cpp \
		$(SERVER_DIR)/sExec.cpp \
		$(SERVER_DIR)/Res.cpp \
		$(SERVER_DIR)/Req.cpp \
		$(SERVER_DIR)/Client.cpp \
		$(SERVER_DIR)/utils.cpp

OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

# --> ANSI COLOR <--
RED		= \e[31m
GREEN	= \e[0;32m
YELLOW	= \e[0;33m
RESET	= \e[0m

all: $(NAME)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "\n"
	@echo "${RED}	█████████████████████████████████████████${RESET}"
	@echo "${GREEN}	█   █ █████ ████   ████ █████ ████  █   █${RESET}"
	@echo "${GREEN}	█   █ █     █   █ █     █     █   █ █   █${RESET}"
	@echo "${GREEN}	█ █ █ ████  ████   ███  ████  ████  █   █${RESET}"
	@echo "${GREEN}	██ ██ █     █   █     █ █     █  █   █ █ ${RESET}"
	@echo "${GREEN}	█   █ █████ ████  ████  █████ █   █   █  ${RESET}"
	@echo "${RED}	█████████████████████████████████████████${RESET}"
	@echo "\n"

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo -e "$(GREEN)Compiling $<$(RESET)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo -e "$(YELLOW)Cleaning...$(RESET)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo -e "$(YELLOW)Removing [$(NAME)]$(RESET)"
	@rm -f $(NAME)

val: re
	@valgrind --leak-check=full ./$(NAME)

re: fclean all

.PHONY: all clean fclean re[vvazzs@viniarch webserver]$
