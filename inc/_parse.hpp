#pragma once

#include "_main.hpp"
#include UTILS_HPP
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sstream>

reHeaders		parseHeaders( std::string str );
serverConfigs	*parseConfigs(char *fileName);
formData		*parseFormData(std::string body);
formUrlEncoded	*parseFormUrlEncoded(std::string url);
