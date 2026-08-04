#pragma once

#include "../_types.hpp"
#include <set>

void confDebbuger(S_CONFIG *conf);
S_CONFIG *parseConfig(char *fileName);
void checkBraces(const std::vector<std::string> &tokens);
void checkValues(const std::vector<std::string> &tokens);
std::vector<std::string> tokenize(const std::string& file);
void validateSyntax(const std::vector<std::string> &tokens);
void checkDuplicates(const std::vector<std::string>& tokens);
void checkSemicolons(const std::vector<std::string> &tokens);
void checkServerBlock(const std::vector<std::string> &tokens);
void checkDirectiveContext(const std::vector<std::string> &tokens);
void checkUnknownDirectives(const std::vector<std::string>& tokens);
void checkDirectiveArguments(const std::vector<std::string> &tokens);
std::vector<std::string> tokenizeHttpRequest(const std::string& request);
