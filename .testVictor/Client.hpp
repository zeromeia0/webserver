enum _STATE {
    COMPLETED,
    R_HEADERS,
    R_CONTENT,
};

class Client {
private:
    int             fd;
    std::string     buffer;
    _STATE          state;
public:
	Request			*REQ;
	Response		*RES;

    Client() : fd(-1), state(R_HEADERS), REQ(NULL), RES(NULL) {};

    Client( const Client &other ) {
        *this = other;
    };

    Client &operator=( const Client &other ) {
        if (this != &other) {
            this->fd = other.fd;
            this->buffer = other.buffer;
            this->state = other.state;
            this->REQ = other.REQ; // NOT DEEP COPY - CAREFUL
            this->RES = other.RES; // NOT DEEP COPY - CAREFUL
        }
        return (*this);
    };

    Client( int newFd ) : fd(newFd), state(R_HEADERS), REQ(NULL), RES(NULL) {};

    ~Client() {
        _reset();
    };

    void _reset() {
        this->buffer.clear();
        this->state = R_HEADERS;
        _free(REQ);
        _free(RES);
    }

    bool receive( const std::string &body ) {
        if (this->state == R_HEADERS)
            this->buffer += body;

        size_t findRes = this->buffer.find("\r\n\r\n");
        if (findRes == std::string::npos)
            return true;

        if (!this->REQ) {
            this->state = R_CONTENT;
            this->REQ = new Request(this->buffer.substr(0, findRes));
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

    int getState() { return this->state; }
    void setState( _STATE newState ) { this->state = newState; }
};
