#pragma once

#include "main.hpp"
#include TYPES_HPP
#include UTILS_HPP
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sstream>

// PARSERS
reHeaders		parseHeaders( std::string str );
serverConfigs	*parseConfigs(char *fileName);
formData		*parseFormData(std::string body);
formUrlEncoded	*parseFormUrlEncoded(std::string body);
