#pragma once

#include "#MAIN.hpp"
#include STRUCTS_HPP

std::string		*getMethodTxt(METHOD code);
METHOD			*getMethodCode(std::string txt);
std::string		*getStatusMsg(int status_code);
std::string		*getMimeExtension(std::string mime);
std::string		*getMimeType(std::string extension);
