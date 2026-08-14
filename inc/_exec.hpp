#pragma once

#include "_main.hpp"
#include <unistd.h>

serverRoute findRoute(std::string uri, std::vector<serverRoute> router);
std::string *cgi(char *bin, char *file, std::map<std::string, std::string> inputs, std::string &payload);
std::string autoindex( std::string path, std::string base_path );
