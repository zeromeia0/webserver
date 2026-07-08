class Server {
private:
    std::vector<struct sConn>	sConns;
    size_t						idx;

	bool						checkSetup();
	void						setupServer();
    void						setOptions();

	void						bindSocket( int port );

	void						listenSocket();

	void						createClient();
	bool						receiveRequest();
	void						respondRequest();
	void						endConn();
	bool						_poll();
	void						pollLoop();

	void						closeServer();

public:
	Server();
    Server &operator=( const Server &other );
    Server( const Server &other );
    ~Server();
};
