#pragma once

#include "_main.hpp"
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

// PARSE
void confDebbuger(serverConfigs *conf);
serverConfigs *parseConfigs(char *fileName);
std::vector<std::string> tokenize(const std::string& file);
void validateSyntax(const std::vector<std::string> &tokens);
std::vector<std::string> tokenizeHttpRequest(const std::string& request);

// EXEC
std::string	intToChar( int value );
char toLower( unsigned char c );
std::string readFileContent( std::string path );
std::string getFileExtension(std::string filename);
void writeFileContent( std::string filename, std::string content );

template <typename T>
std::string vectorToListString(std::vector<T> vector) {
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

template <typename T>
bool valueInContainer(std::string *value, std::vector<T> container) {
	if (!value)
		return (false);
	for (size_t i = 0; i < container.size(); i++) {
		if (container[i] == *value) {
			return (true);
		}
	}
	return (false);
}

template <typename T, typename U>
std::string mapToJsonString(std::map<T, U> map) {
	std::string ret;
	ret.append("{");
	for (typename std::map<T, U>::iterator it = map.begin(); it != map.end(); ++it) {
		if (it != map.begin())
			ret.append(", ");
        ret.append("\"" + it->first + "\": \"" + it->second + "\"");
	}
	ret.append("}");
	return (ret);
}
