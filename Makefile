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
CXXFLAGS		= -Wall -Wextra -Werror -g -std=c++98 -I inc

NAME			= webserver
OBJ_DIR			= obj

# ########################################
# SRC
# ########################################

SRC_CHECK = \
	src/check/check_configs.cpp

SRC_DEBUG = \
	src/debug/debug_configs.cpp

SRC_EXEC = \
	src/exec/Client/Client.cpp \
	src/exec/Connection/Connection.cpp \
	src/exec/Server/ServerIn.cpp \
	src/exec/Server/ServerLoop.cpp \
	src/exec/Server/ServerOut.cpp \
	src/exec/Server/ServerSetup.cpp

SRC_PARSE = \
	src/parse/parse_configs.cpp \
	src/parse/parse_form_data.cpp \
	src/parse/parse_form_urlencoded.cpp \
	src/parse/parse_headers.cpp

SRC_UTILS = \
	src/utils/exec.cpp \
	src/utils/map.cpp \
	src/utils/parse.cpp \
	src/utils/utils.cpp

SRC = \
	src/main.cpp \
	$(SRC_CHECK) \
	$(SRC_DEBUG) \
	$(SRC_EXEC) \
	$(SRC_PARSE) \
	$(SRC_UTILS)

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
