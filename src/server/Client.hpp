class Client {
public:
	int             fd;
	std::string     buffer;
	reqState        state;
    Re             *REQ;
    Re             *RES;

    // OCF
    Client();
    Client( int newFd );
    Client( const Client &other );
    Client &operator=( const Client &other );
    ~Client();

    void _reset();
    bool receive( const std::string &body );
};
