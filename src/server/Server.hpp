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
	void						respondRequest();
	void						loopServer();

    // METHODS
    void                        GET( int fd, const std::string &path );
    void                        POST();
    void                        DELETE();

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
    struct routeConfig
    {
        routeConfig() : autoindex(false), uploadEnabled(false) {}
        std::string path;
        std::string root;
        std::string index;
        std::string uploadPath;
        std::string redirect;
        bool autoindex;
        bool uploadEnabled;
        std::vector<std::string> methods;
        std::map<std::string, std::string> cgi;
    };

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
    routeConfig          *rConf;

};
