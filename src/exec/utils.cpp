#include "_utils.hpp"

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

void printVector(std::vector<std::string> vec) {
    for (size_t i = 0; i < vec.size(); i++)
        std::cout << vec[i] << std::endl;
}

void printMap(std::map<std::string, std::string> map) {
	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << " -> " << it->second << std::endl;
	}
}

void writeFileContent( std::string filename, std::string content ) {
	std::fstream file(filename.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
    file << content;
    file.close();
};

bool valueInContainer(std::string *value, std::vector<std::string> container) {
	if (!value)
		return (false);
	for (size_t i = 0; i < container.size(); i++) {
		if (container[i] == *value) {
			return (true);
		}
	}
	return (false);
}
