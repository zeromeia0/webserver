#pragma once

#include "#MAIN.hpp"
#include <unistd.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <sys/wait.h>
#include <fcntl.h>

sRoute			findRoute(std::string uri, std::vector<sRoute> router);
std::string		*cgi(char *bin, char *file, std::map<std::string, std::string> inputs, std::string &payload);
std::string		autoindex( std::string path, std::string base_path );
