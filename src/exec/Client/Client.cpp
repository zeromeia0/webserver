#include "Client.hpp"

Client::Client() : fd(-1), state(READING_HEADERS), REQ(NULL), RES(NULL) {}

Client::Client( const Client &other ) {
    *this = other;
};

Client &Client::operator=( const Client &other ) {
    if (this != &other) {
        this->fd = other.fd;
        this->state = other.state;
        this->REQ = other.REQ; // NOT DEEP COPY - CAREFUL
        this->RES = other.RES; // NOT DEEP COPY - CAREFUL
    }
    return (*this);
};

Client::Client( int newFd ) : fd(newFd), state(READING_HEADERS), REQ(NULL), RES(NULL) {};

Client::~Client() { _reset(); };

void Client::_reset() {
    this->state = READING_HEADERS;
    _free(REQ);
    _free(RES);
}
