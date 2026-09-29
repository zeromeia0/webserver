#pragma once

#include "main.hpp"
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <cctype>

// ############################################################
// 							PARSE
// ############################################################

std::vector<std::string>	tokenize(const std::string& file);
void						validateSyntax(const std::vector<std::string> &tokens);

template					<typename T>
std::string					vectorToListString(std::vector<T> vector) {
	std::string ret;
	ret.append("[");
	for (size_t i = 0; i < vector.size(); i++) {
		std::stringstream ss;
		ss << vector[i];
		ret.append("\"" + ss.str() + "\"");
		if (i + 1 != vector.size())
			ret.append(", ");
	}
	ret.append("]");
	return (ret);
}

// ############################################################
// 							EXEC
// ############################################################

std::string					intToChar( int value );
char						toLower( unsigned char c );
std::string					readFileContent( std::string path );
std::string					getFileExtension(std::string filename);
bool						writeFileContent( std::string filename, std::string content );
std::string					decodeUrl(std::string url);

template					<typename T>
bool						valueInContainer(std::string value, std::vector<T> container) {
	if (value.empty())
		return (false);
	for (size_t i = 0; i < container.size(); i++) {
		if (container[i] == value) {
			return (true);
		}
	}
	return (false);
}
