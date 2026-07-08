#include "../main.hpp"

Client::Client() : fd(-1), state(R_HEADERS), REQ(NULL), RES(NULL) {}

Client::Client( const Client &other ) {
    *this = other;
};

Client &Client::operator=( const Client &other ) {
    if (this != &other) {
        this->fd = other.fd;
        this->buffer = other.buffer;
        this->state = other.state;
        this->REQ = other.REQ; // NOT DEEP COPY - CAREFUL
        this->RES = other.RES; // NOT DEEP COPY - CAREFUL
    }
    return (*this);
};

Client::Client( int newFd ) : fd(newFd), state(R_HEADERS), REQ(NULL), RES(NULL) {};

Client::~Client() { _reset(); };

void Client::_reset() {
    this->buffer.clear();
    this->state = R_HEADERS;
    _free(REQ);
    _free(RES);
}

bool Client::receive( const std::string &body ) {
    if (this->state == R_HEADERS)
        this->buffer += body;

    size_t findRes = this->buffer.find("\r\n\r\n");
    if (findRes == std::string::npos)
        return true;

    if (!this->REQ) {
        this->state = R_CONTENT;
        this->REQ = new Req(this->buffer.substr(0, findRes));
        this->REQ->addContent(this->buffer.substr(findRes + 4));
    } else {
        this->REQ->addContent(body);
    }

    const char *conLen = REQ->getHeader("content-length");
    if ((conLen ? std::atoi(conLen) : 0) == REQ->getContentLen()) {
        this->state = COMPLETED;
        return false;
    }

    return (true);
};

// GETTERS
int Client::getState() { return this->state; }

// SETTERS
void Client::setState( reqState newState ) { this->state = newState; }
