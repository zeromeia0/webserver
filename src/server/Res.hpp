class Res : public ARe {
private:
    int									resStatusCode;
public:
	Res();
    Res( const Res &other );
    Res &operator=( const Res &other );
    ~Res();

	void stringify();

	// ####### GETTERS
	int getStatusCode();

	// ####### SETTERS
	void setStatusCode( int newStatusCode ) { this->resStatusCode = newStatusCode; };

    void display();

	void respond( int fd ) {
		addHeader("Content-Length", intToChar(getContent().size()));
		stringify();
		display();
		send(fd, body.c_str(), body.size(), 0);
	};

};
