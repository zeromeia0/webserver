#pragma once

#include "#MAIN.hpp"
#include <unistd.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <cerrno>
#include <climits>
#include <dirent.h>

sRoute			findRoute( const std::string &uri, const std::vector<sRoute> &router );
std::string		autoindex( std::string path, std::string base_path );