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
	std::fstream file(path.c_str(), std::ios::in | std::ios::binary);
	if (!file.is_open())
		return ("");
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return (content);
}

std::string getFileExtension(std::string filename) {
	size_t pos = filename.find_last_of('.');
	if (pos == std::string::npos)
		return ("");
	std::string ret = filename.substr(pos);
	return (ret);
}

bool writeFileContent( std::string filename, std::string content ) {
	std::fstream file(filename.c_str(), std::fstream::out | std::fstream::trunc);
	file << content;
	if (file.fail())
		return (false);
	file.close();
	return (true);
};

std::string decodeUrl(std::string url) {
	std::string new_str;
	for (size_t i = 0; i < url.size(); i++) {
		if (url[i] == '%' && i + 2 < url.size()) {
			std::istringstream iss(url.substr(i + 1, 2));
			int c;
			iss >> std::hex >> c;
			new_str += static_cast<char>(c);
			i += 2;
		} else if (url[i] == '+') {
			new_str += ' ';
		} else {
			new_str += url[i];
		}
	}
	return new_str;
}
