#pragma once

#include "main.hpp"
#include STRUCTS_HPP

std::string		getMethodTxt( RE_METHOD code );
std::string		getStatusMsg( int status_code );
RE_METHOD		*getMethodCode( std::string txt );
std::string		*getMimeExtension( std::string mime );
std::string		*getMimeType( std::string extension );
