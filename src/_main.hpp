#pragma once

#include "_types.hpp"
#include "exec/Server/Server.hpp"

#define LOG(categ, msg) std::cout << "[" << categ << "] " << msg << std::endl

// ########## TO PARSE ##########
#define HTTP_VERSION		"HTTP/1.1"
#define SIN_FAMILY			AF_INET
#define SIN_ADDR			INADDR_ANY
#define BUFF_SIZE			1024
#define ROOT				"var/www"
#define UPLOAD_PATH			"var/www/uploads"
#define CGI_PATH			"var/www/cgi-bin"
#define F_404				"/404.html"
#define CONN_REQS_Q			100
// ##############################

// ##########################################################################################
// # SERVER
// ##########################################################################################

// typedef enum E_MODE {
// 	DEV,
// 	PROD,
// } MODE;
// extern MODE mode;

template <typename T>
void _free(T *&ptr) {
    if (!ptr)
		return;
	delete ptr;
	ptr = NULL;
}

