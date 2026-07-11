#include "../main.hpp"

Res::Res() : resStatusCode(0) {};

Res::Res( const Res &other ) { *this = other; };

Res &Res::operator=( const Res &other ) {
	if (this != &other) {
		this->resStatusCode = other.resStatusCode;
		ARe::operator=(other);
	}
	return (*this);
};

Res::~Res() {};

void Res::stringify() {
	body =
		version + " "
		+ intToChar(resStatusCode) + " "
		+ (resStatusCode == 200 ? "OK" : "") + " "
		+ "\r\n";
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
		body = body + it->first + ": " + it->second + "\r\n";
	body = body + "\r\n";
	body = body + content;
};

// ####### GETTERS
int Res::getStatusCode() { return (this->resStatusCode); }

void Res::display() {
	std::cout << "---------- RESPONSE ----------" << std::endl;
	std::cout << "resStatusCode: " << this->resStatusCode << std::endl;
	ARe::ADisplay();
}
