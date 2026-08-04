#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <set>
#include <map>

char toLower(unsigned char c);
std::string	intToChar(int value);
void printVector(std::vector<std::string> vec);
std::string readFileContent(std::string path);
std::string getFileExtension(std::string filename);
void printMap(std::map<std::string, std::string> map);
void writeFileContent(std::string path, std::string content);
bool valueInContainer(std::string *value, std::vector<std::string> container);
