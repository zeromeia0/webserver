#include "../main.hpp"

Res::Res() : resStatusCode(0) {};

Res::Res( std::string path ) : resStatusCode(0) {
	if (path == "/")
		path = "/index.html";
	path = ROOT + path;
	LOG("PATHHHHH", path);
	version = "HTTP/1.1";
	if (getFileContent( path ))
		resStatusCode = 200;
	else
		resStatusCode = 404;
	addHeader("Content-Type", "text/html");
	addHeader("Content-Length", intToChar(std::strlen(content.c_str())));
	body = toString();
	display();
};

Res::Res( const Res &other ) { *this = other; };

Res &Res::operator=( const Res &other ) {
	if (this != &other) {
		this->resStatusCode = other.resStatusCode;
		ARe::operator=(other);
	}
	return (*this);
};

Res::~Res() {};

std::string Res::toString() {
	std::string res =
		version + " "
		+ intToChar(resStatusCode) + " "
		+ (resStatusCode == 200 ? "OK" : "NOT OK") + " "
		+ "\r\n";
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
		res = res + it->first + ": " + it->second + "\r\n";
	res = res + "\r\n";
	res = res + content;
	// res = res + "\0"; // Useless
	return (res);
};

// ####### Getters
int Res::getStatusCode() { return (this->resStatusCode); }

void Res::display() {
	std::cout << "---------- RESPONSE ----------" << std::endl;
	std::cout << "resStatusCode: " << this->resStatusCode << std::endl;
	ARe::ADisplay();
}

bool Res::getFileContent( std::string &path ) {

	bool ret = true;
	if (access(path.c_str(), F_OK) < 0) {
		path = ROOT + std::string(F_404);
		ret = false;
	}

	std::fstream file(path.c_str());
	// if (!file.is_open())
	// 	throw std::out_of_range("ERROR");
	
	char c;
	while(file.get(c))
		content = content + c;

	return (ret);
};
