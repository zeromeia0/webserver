#pragma once

#include "main.hpp"

std::string	*getMethodTxt(eMethod code);
eMethod		*getMethodCode(std::string txt);
std::string	*getStatusMsg(int status_code);
std::string	*getMimeExtension(std::string mime);
std::string	*getMimeType(std::string extension);
