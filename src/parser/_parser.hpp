#pragma once

#include "../_types.hpp"
#include "../_main.hpp"
#include "_utils.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sstream>

// PARSERS
HEADERS			parseHead( std::string str );
S_CONFIG		*parseConfig(char *fileName);
formData		*parseFormData(std::string body);
formUrlEncoded	*parseformUrlEncoded(std::string body);
