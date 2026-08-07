#pragma once

#define CHECK_HPP		"_check.hpp"
#define DEBUG_HPP		"_debug.hpp"
#define EXEC_HPP		"_exec.hpp"
#define MAP_HPP			"_map.hpp"
#define PARSE_HPP		"_parse.hpp"
#define TYPES_HPP		"_types.hpp"
#define UTILS_HPP		"_utils.hpp"
#define CLIENT_HPP		"Client.hpp"
#define CONNECTION_HPP	"Connection.hpp"
#define RE_HPP			"Re.hpp"
#define REQUEST_HPP		"Request.hpp"
#define RESPONSE_HPP	"Response.hpp"
#define SERVER_HPP		"Server.hpp"

#include TYPES_HPP
#include <iostream>

#define DEBUG 1
#define LOG(categ, msg) \
if (DEBUG || std::string(categ) != "DEBUG") \
	std::cout << "[" << categ << "] " << msg << std::endl

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

