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

CXX				= c++
CXXFLAGS		= -Wall -Wextra -Werror -g -std=c++98

NAME			= webserver
OBJ_DIR			= obj

PARSER_DIR		= ./src/parser
SRC_PARSER		= $(PARSER_DIR)/conf_validation.cpp \
				$(PARSER_DIR)/config.cpp \
				$(PARSER_DIR)/debugger.cpp \
				$(PARSER_DIR)/form_data.cpp \
				$(PARSER_DIR)/form_urlencoded.cpp \
				$(PARSER_DIR)/head.cpp \
				$(PARSER_DIR)/tokenize.cpp \

EXEC_DIR		= ./src/exec
SRC_EXEC		= $(EXEC_DIR)/utils.cpp \
				$(EXEC_DIR)/Re/cgi.cpp \
				$(EXEC_DIR)/Re/autoindex.cpp \
 				$(EXEC_DIR)/Client/Client.cpp \
				$(EXEC_DIR)/Server/Setup.cpp \
				$(EXEC_DIR)/Server/LOOP.cpp \
				$(EXEC_DIR)/Server/IN.cpp \
				$(EXEC_DIR)/Server/OUT.cpp

ROOT_DIR		= ./src
SRC_ROOT		= $(ROOT_DIR)/main.cpp \
				$(ROOT_DIR)/MAP.cpp

SRC = $(SRC_ROOT) $(SRC_PARSER) $(SRC_EXEC)

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
