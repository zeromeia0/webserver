#pragma once

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <csignal>
#include <sstream>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <exception>
#include <fstream>
#include <iomanip>
#include <list>
#include <set>

#define LOG(categ, msg) if (mode == DEV || std::string(categ) != "DEBUG") \
	std::cout << "[" << categ << "] " << msg << std::endl

// ########## TO PARSE ##########
#define SIN_FAMILY			AF_INET
#define SIN_ADDR			INADDR_ANY
#define BUFF_SIZE			1024
#define ROOT				"var/www"
#define F_404				"/404.html"
#define CONN_REQS_Q			100
// ##############################

// ##########################################################################################
// # SERVER
// ##########################################################################################

typedef enum E_MODE {
	DEV,
	PROD,
} MODE;
extern MODE mode;

enum reqState {
    COMPLETED,
    R_HEADERS,
    R_CONTENT,
};

template <typename T>
void _free(T *&ptr) {
    if (!ptr)
		return;
	delete ptr;
	ptr = NULL;
}

std::string intToChar( int value );
char toLower( unsigned char c );

#include "./server/ARe.hpp"
#include "./server/Client.hpp"
#include "./server/Server.hpp"

struct sConn {
	struct pollfd	pfd;
	Client			*c;
};

// ##########################################################################################
// # PARSER
// ##########################################################################################

void parse(Server::serverConfig *conf, char *fileName);

// PARSING .CONF SHIT
std::string validateFile(char *fileName);
void getInfo(Server::serverConfig *conf);
void confDebbuger(Server::serverConfig *conf);
void confAssignValue(Server::serverConfig *server, Server::routeConfig *&currentRoute, const std::vector<std::string> &tokens, size_t i);
std::vector<std::string> tokenize(const std::string& file);
std::vector<std::string> tokenizeHttpRequest(const std::string& request); //TRY TO POLYMORPHISM THIS SHIT

// TRYING OUT NOW
void validateSemantics(const std::vector<std::string> &tokens);
void checkListen(const Server::serverConfig *conf);
void checkMethods(const Server::serverConfig *conf);
void checkRoutes(const Server::serverConfig *conf);
void checkCgi(const Server::serverConfig *conf);

void checkUnknownDirectives(const std::vector<std::string> &tokens);

bool isDirective(const std::string &token);
bool isNumber(const std::string &token);
bool isMethod(const std::string &token);

size_t findSemicolon(const std::vector<std::string> &tokens, size_t start);
size_t countArgsUntilSemicolon(const std::vector<std::string> &tokens, size_t start);
void expectArgs(const std::vector<std::string> &tokens, size_t i, size_t expected);
void checkBraces(const std::vector<std::string> &tokens);
void checkServerBlock(const std::vector<std::string> &tokens);
void checkDirectiveContext(const std::vector<std::string> &tokens);
void checkDirectiveArguments(const std::vector<std::string> &tokens);
void checkSemicolons(const std::vector<std::string> &tokens);
void checkValues(const std::vector<std::string> &tokens);
void validateSyntax(const std::vector<std::string> &tokens);
void checkDuplicates(const std::vector<std::string>& tokens);
