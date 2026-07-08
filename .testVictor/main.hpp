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

// ########## TO PARSE ##########

#define SIN_FAMILY			AF_INET
#define SIN_ADDR			INADDR_ANY
#define BUFF_SIZE			1024
#define ROOT				"www"
#define F_404				"/404.html"
#define	ARR_PORTS			{4243, 4244, 4245}
extern std::vector<int>		PORTS;
#define CONN_REQS_Q			100
// ##############################

typedef enum E_MODE {
	DEV,
	PROD,
} MODE;

extern MODE mode;

#define LOG(categ, msg) if (mode == DEV || std::string(categ) != "DEBUG") \
	std::cout << "[" << categ << "] " << msg << std::endl

template <typename T>
void _free(T *&ptr) {
    if (!ptr)
		return;
	delete ptr;
	ptr = NULL;
}

std::string intToChar( int value );
char toLower( unsigned char c );
std::vector<std::string> tokenize(const std::string& request);

#include "ARe.hpp"
#include "Client.hpp"
#include "Server.hpp"

struct sConn {
	struct pollfd	pfd;
	Client			*c;
};
