# COLORS
RED			= \e[31m
GREEN		= \e[32m
END			= \e[0m

CXX			= c++
CXXFLAGS	= -std=c++98 -Wall -Wextra -Werror -g

NAME		= webserver
SRCS		= main.cpp Server.cpp Client.cpp utils.cpp
OBJS		= $(SRCS:%.cpp=%.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "\n"
	@echo "${RED}	█████████████████████████████████████████${END}"
	@echo "${GREEN}	█   █ █████ ████   ████ █████ ████  █   █${END}"
	@echo "${GREEN}	█   █ █     █   █ █     █     █   █ █   █${END}"
	@echo "${GREEN}	█ █ █ ████  ████   ███  ████  ████  █   █${END}"
	@echo "${GREEN}	██ ██ █     █   █     █ █     █  █   █ █ ${END}"
	@echo "${GREEN}	█   █ █████ ████  ████  █████ █   █   █  ${END}"
	@echo "${RED}	█████████████████████████████████████████${END}"
	@echo "\n"

%.o:%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
