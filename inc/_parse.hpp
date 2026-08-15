#pragma once

#include "#MAIN.hpp"
#include UTILS_HPP
#include MAP_HPP
#include <fcntl.h>
#include <unistd.h>

sHeaders			parseHeaders(std::string str);
sConfigs			*parseConfigs(char *fileName);
sFormData			*parseFormData(std::string body);
sFormUrlEncoded		*parseFormUrlEncoded(std::string url);
