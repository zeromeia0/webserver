class Server {
private:
    std::vector<struct sConn>	sConns;
    size_t						idx;

    // ##########################################
    // SETUP
    // ##########################################

	bool						checkSetup();
	void						setupServer();
    void						setOptions();

	void						bindSocket( int port );

	void						listenSocket();
	bool						_poll();
    void                        _pollin();
    void                        _pollout();

	void						closeServer();

public:
	// OCF
	Server();
    Server( char *confFileName );
    Server &operator=( const Server &other );
    Server( const Server &other );
    ~Server();

    void init();
    void start();

    // STRUCTS
    struct serverConfig
    {
        std::vector<int> listenPorts;
        std::string host;
        std::string serverName;
        size_t clientMaxBodySize;
        std::map<int, std::string> errorPages;
        std::vector<routeConfig> router;
        std::vector<std::string> confFile;
    };

    serverConfig         *sConf;

    // ##########################################
    // EXEC
    // ##########################################
	void						createClient();
	void						endConn();

	void						LOOP();

	void respond( int statusCode );

    routeConfig findRoute( std::string path );

};
