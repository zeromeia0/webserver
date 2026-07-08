#include "../main.hpp"

Req::Req() {};

Req::Req( const Req &other ) { *this = other; };

Req &Req::operator=( const Req &other ) {
    if (this != &other) {
        this->reqMethod = other.reqMethod;
        this->reqPath = other.reqPath;
        ARe::operator=(other);
    }
    return (*this);
};

Req::Req( const std::string &newHeaders) {
    LOG("DEBUG", newHeaders);
    std::vector<std::string> tokens = tokenizeHttpRequest(newHeaders);
    int i = 0;
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
        if (i == 0)                 { this->reqMethod = *it; }
        else if (i == 1)            { this->reqPath = *it; }
        else if (i == 2)            { this->version = *it; }
        else if (*it == "\\r\\n")   { ; }
        else {
            std::pair<std::string, std::string> tmp;
            tmp.first = *it;
            it++;
            if (!(*it == ":"))
                break;
            it++;
            while (1) {
                tmp.second += *it;
                if (it + 1 == tokens.end() || *(it + 1) == "\\r\\n")
                    break;
                it++; i++;
                tmp.second += " ";
            }
            std::transform(tmp.first.begin(), tmp.first.end(), tmp.first.begin(), tolower);
            this->headers.insert(tmp);
        }
        i++;
    }
    display();
};

// ####### GETTERS
std::string Req::getMethod() { return (this->reqMethod); };
std::string Req::getPath() { return (this->reqPath); };

void Req::display() {
    std::cout << "---------- REQUEST ----------" << std::endl;
    std::cout << "reqMethod: " << this->reqMethod << std::endl;
    std::cout << "reqPath: " << this->reqPath << std::endl;
    ARe::ADisplay();
}

Req::~Req() {}
