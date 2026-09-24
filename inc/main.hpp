#pragma once

#define CHECK_HPP		"_check.hpp"
#define DEBUG_HPP		"_debug.hpp"
#define ENUMS_HPP		"_enums.hpp"
#define EXEC_HPP		"_exec.hpp"
#define MAP_HPP			"_maps.hpp"
#define PARSE_HPP		"_parse.hpp"
#define STRUCTS_HPP		"_structs.hpp"
#define UTILS_HPP		"_utils.hpp"
#define CLIENT_HPP		"Client.hpp"
#define CONNECTION_HPP	"Connection.hpp"
#define RE_HPP			"Re.hpp"
#define REQUEST_HPP		"Request.hpp"
#define RESPONSE_HPP	"Response.hpp"
#define SERVER_HPP		"Server.hpp"

#include ENUMS_HPP
#include STRUCTS_HPP
#include <iostream>
#include <string>

#define SIN_FAMILY		AF_INET
#define SIN_ADDR		INADDR_ANY
#define BUFF_SIZE		65536
#define CONN_REQS_Q		1024
#define TIMEOUT			30
#define DEF_MAX_BODY	100000000

#define DEBUG			1
#if DEBUG
#define LOG(categ, msg) do {\
	std::cerr << "[" << categ << "] " << msg << std::endl;\
} while(0)
#else
#define LOG(categ, msg) do {} while(0)
#endif

#define THROW(msg) throw std::runtime_error("Error: " + std::string(msg));

template <typename T>
void _free(T *&ptr) {
	if (!ptr)
		return;
	delete ptr;
	ptr = NULL;
}
