#pragma once

#include "main.hpp"
#include <set>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>

void	validateConfigs(const std::vector<sConfigs*> &confs);
void	validateSyntax(const std::vector<std::string> &tokens);
int		checkHeaders(sHeaders *headers);
