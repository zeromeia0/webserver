class Res : public ARe {
private:
    int									resStatusCode;
public:
	Res();
    Res( const Res &other );
    Res &operator=( const Res &other );
    ~Res();

	struct vars_s {
		std::string method;
		routeConfig route;
		int fd;
		std::string reqPath;
		std::string path;
		std::string indexedPath;
		DIR *dir;
		int dir_errno;
	};
	vars_s vars; 

	void stringify();

	// ####### GETTERS
	int getStatusCode();

	// ####### SETTERS
	void setStatusCode( int newStatusCode ) { this->resStatusCode = newStatusCode; };

    void display();

	void respond( int statusCode ) {
		setStatusCode(statusCode);
		std::string *content = NULL;
		switch (statusCode) {
			case 200:
				if (getPath().empty() && vars.method == "GET") {
					autoIndex(); // Check for errors inside function
				} else if (!getPath().empty()) {
					content = getFileContent(getPath());
					addContent(*content);
					delete content;
				} else {
					addContent("200 - OK");
				}
				break;
			case 301:
				addHeader("Location", getPath());
				addContent("301 - Moved Permanently");
				break;
			case 403:
				addContent("403 - Forbidden");
				break;
			case 404:
				addContent("404 - Not found");
				break;
			case 405:
				addContent("405 - Method Not Allowed");
				break;
			case 500:
				addContent("500 - Internal server error");
				break;
			default:
				addContent("500 - Internal server error");
				break;
		}
		setVersion("HTTP/1.1");
		if (!getContent().empty()) {
			addHeader("Content-Type", "text/html"); // TO UPDATE
			addHeader("Content-Length", intToChar(getContent().size()));
		}
		stringify();
		display();
		send(vars.fd, body.c_str(), body.size(), 0); // CHECK THE BYTES SENT IF MULTI PACKAGE
	};

	void autoIndex() {
		dirent *_dirent = readdir(vars.dir);
		addContent("<body>\r\n");
		while ((_dirent = readdir(vars.dir))) {
			std::string cont = std::string(_dirent->d_name);
			addContent("<div>");
			addContent("<a href=" + vars.reqPath + "/" + cont + ">");
			addContent(cont);
			addContent("</a>");
			addContent("</div>");
		}
		addContent("</body>\r\n");
	};

};
