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
#include <dirent.h>

#define LOG(categ, msg) if (mode == DEV || std::string(categ) != "DEBUG") \
	std::cout << "[" << categ << "] " << msg << std::endl

#define HTTP_VERSION "HTTP/1.1"

// ########## TO PARSE ##########
#define SIN_FAMILY			AF_INET
#define SIN_ADDR			INADDR_ANY
#define BUFF_SIZE			1024
#define ROOT				"var/www"
#define UPLOAD_PATH			"var/www/uploads"
#define CGI_PATH			"var/www/cgi-bin"
#define F_404				"/404.html"
#define CONN_REQS_Q			100
// ##############################
std::vector<std::string> tokenizeHttpRequest(const std::string& request); //TRY TO POLYMORPHISM THIS SHIT

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

struct formData {
        std::string boundaryLimiter;
        std::string name;
        std::string filename;
};

struct routeConfig
{
	routeConfig() : autoindex(false), uploadEnabled(false) {}
	std::string path;
	std::string root;
	std::string index;
	std::string uploadStore;
	std::string redirect;
	bool autoindex;
	bool uploadEnabled;
	std::vector<std::string> methods;
	std::map<std::string, std::string> cgi;
};

template <typename T>
void _free(T *&ptr) {
    if (!ptr)
		return;
	delete ptr;
	ptr = NULL;
}

std::string	intToChar( int value );
char toLower( unsigned char c );
formData parseBoundary(std::string body);
std::string *readFileContent( std::string path );
void writeFileContent( std::string path, std::string content );
bool valueInContainer(std::string value, std::vector<std::string> container);
void printVector(std::vector<std::string> vec);
std::string getStatusMsg(int code);
std::string getFileExtension(std::string filename);
std::string *getMime(std::string extension);
std::string *getExtension(std::string mime);
std::string cgi(const char *file, char **args, char **envp);

#include "./server/Re.hpp"
#include "./server/Client.hpp"
#include "./server/Server.hpp"

struct sConn {
	struct pollfd	poll_fd;
	Client			*client;
};

std::string autoindex( std::string path, std::string base_path);


// ##########################################################################################
// # PARSER
// ##########################################################################################

std::string getFileName( std::string content );

void parse(Server::serverConfig *conf, char *fileName);

// PARSING .CONF SHIT
std::string validateFile(char *fileName);
void getInfo(Server::serverConfig *conf);
void confDebbuger(Server::serverConfig *conf);
void confAssignValue(Server::serverConfig *server, routeConfig *&currentRoute, const std::vector<std::string> &tokens, size_t i);
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
