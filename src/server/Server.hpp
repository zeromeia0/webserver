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

	void						createClient();
	void						endConn();
	bool						_poll();

	void						closeServer();

    // ##########################################
    // EXEC
    // ##########################################

	bool						receiveRequest();
	void						loopServer();

    // METHODS
    void                        GET( int fd, routeConfig route, Res &res, std::string path );
    void                        POST( int fd, routeConfig route, std::string path );

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

    routeConfig findRoute( std::string path );
    bool isMethodAllowed(std::string reqMethod, std::vector<std::string> routeMethods) {
        for (size_t i = 0; i < routeMethods.size(); i++) {
            if (routeMethods[i] == reqMethod) {
                return (true);
            }
        }
        return (false);
    }

};
