#pragma once

#include "main.hpp"
#include UTILS_HPP
#include MAP_HPP
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <algorithm>

sHeaders			*parseHeaders(std::string str);
sConfigs			*parseConfigs(char *fileName);
std::string			parseFormData(std::string payload, std::string contentType);
sFormUrlEncoded		parseFormUrlEncoded(std::string url);
std::string			parseChunkedBody( std::string *newBytes, std::string *previousBytes, int *status );
