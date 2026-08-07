#pragma once

#include <iostream>

#define DEBUG 1
#define LOG(categ, msg) \
if (DEBUG || std::string(categ) != "DEBUG") \
	std::cout << "[" << categ << "] " << msg << std::endl

// HPP FILES
#define CLIENT_HPP		"Client.hpp"
#define CONNECTION_HPP	"Connection.hpp"
#define EXEC_HPP		"exec.hpp"
#define MAP_HPP			"map.hpp"
#define PARSE_HPP		"parse.hpp"
#define RE_HPP			"Re.hpp"
#define REQUEST_HPP		"Request.hpp"
#define RESPONSE_HPP	"Response.hpp"
#define SERVER_HPP		"Server.hpp"
#define TYPES_HPP		"types.hpp"
#define UTILS_HPP		"utils.hpp"


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

template <typename T>
void _free(T *&ptr) {
    if (!ptr)
		return;
	delete ptr;
	ptr = NULL;
}

