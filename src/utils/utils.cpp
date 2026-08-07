#include "utils.hpp"

std::string	intToChar( int value ) {
    std::stringstream iss;
    iss << value;
    std::string str;
    iss >> str;
    return (str);
}

char toLower( unsigned char c ) {
    return std::tolower(c);
}

std::string readFileContent( std::string path ) {

	std::string content;
	std::fstream file(path.c_str());
	if (!file.is_open())
        return (content);

    char c;
	while(file.get(c))
		content = content + c;

	return (content);
};

std::string getFileExtension(std::string filename) {
	return (filename.substr(filename.find_last_of('.')));
}

void writeFileContent( std::string filename, std::string content ) {
	std::fstream file(filename.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
    file << content;
    file.close();
};
