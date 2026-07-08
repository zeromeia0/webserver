class Client {
private:
    int             fd;
    std::string     buffer;
    reqState        state;
public:
    Req             *REQ;
    Res             *RES;

    // OCF
    Client();
    Client( int newFd );
    Client( const Client &other );
    Client &operator=( const Client &other );
    ~Client();

    void _reset();
    bool receive( const std::string &body );

    // GETTERS
    int getState();

    // SETTERS
    void setState( reqState newState );
};
