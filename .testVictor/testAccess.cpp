#include <iostream>
#include <unistd.h>

int main() {
	std::string fileName;

	fileName = "www/test.html";
	std::cout << fileName << ": " << access(fileName.c_str(), F_OK) << std::endl;

	fileName = "www/test32432.html";
	std::cout << fileName << ": " << access(fileName.c_str(), F_OK) << std::endl;

	return (1);
}