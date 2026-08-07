#pragma once

#include "_main.hpp"
#include <unistd.h>

std::string *cgi(const char *file, char **args, char **envp, const std::string &payload);
std::string autoindex( std::string path, std::string base_path );
