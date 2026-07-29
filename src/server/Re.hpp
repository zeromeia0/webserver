class Re {
public:
	std::string							body;
    std::string                         version;
	std::string							path;
    std::map<std::string, std::string>  headers;
    std::string 						method;
    int			 						status_code;
    std::string                         payload;
    size_t								payloadLen;
    bool								is_autoindex;
    bool								is_redirect;

	// ####### OCF
	Re() : status_code(0), payloadLen(0), is_autoindex(false), is_redirect(false) {};
	Re( const Re &other ) { *this = other; };
	Re &operator=( const Re &other ) {
		if (this != &other) {
			this->body = other.body;
			this->version = other.version;
			this->path = other.path;
			this->headers = other.headers;
			this->method = other.method;
			this->status_code = other.status_code;
			this->payload = other.payload;
			this->payloadLen = other.payloadLen;
			this->is_autoindex = other.is_autoindex;
			this->is_redirect = other.is_redirect;
		}
		return (*this);
	};
	~Re() {};

    const char *getHeader( std::string strKey ) {
		std::map<std::string, std::string>::iterator it = headers.find(strKey);
		if (it == this->headers.end())
			return (NULL);
        return (it->second.c_str());
    }

	void addHeaders(const std::string &newHeaders) {
		std::vector<std::string> tokens = tokenizeHttpRequest(newHeaders);
		int i = 0;
		for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
			if (i == 0)                 { this->method = *it; }
			else if (i == 1)            { this->path = *it; }
			else if (i == 2)            { this->version = *it; }
			else if (*it == "\\r\\n")   { ; }
			else {
				std::pair<std::string, std::string> tmp;
				tmp.first = *it;
				it++;
				if (!(*it == ":"))
					break;
				it++;
				while (1) {
					tmp.second += *it;
					if (it + 1 == tokens.end() || *(it + 1) == "\\r\\n")
						break;
					it++; i++;
					tmp.second += " ";
				}
				std::transform(tmp.first.begin(), tmp.first.end(), tmp.first.begin(), tolower);
				this->headers.insert(tmp);
			}
			i++;
		}
	};
	void addHeader(std::string strKey, std::string strValue) {
		headers.insert(std::pair<std::string, std::string>(strKey, strValue));
	};
	void addPayload(const std::string &newContent) {
		this->payload = this->payload + newContent;
		this->payloadLen = payload.length();
	};

	void autoIndex(DIR *dir) {
		dirent *_dirent = readdir(dir);
		this->addPayload("<body>\r\n");
		while ((_dirent = readdir(dir))) {
			std::string cont = std::string(_dirent->d_name);
			this->addPayload("<div>");
			this->addPayload("<a href=" + path + "/" + cont + ">");
			this->addPayload(cont);
			this->addPayload("</a>");
			this->addPayload("</div>");
		}
		this->addPayload("</body>\r\n");
	};	

	void stringify() {
		this->body =
			this->version + " "
			+ intToChar(this->status_code) + " "
			+ getStatusMsg(this->status_code) + " "
			+ "\r\n";
		for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); ++it)
			this->body = this->body + it->first + ": " + it->second + "\r\n";
		this->body = this->body + "\r\n";
		this->body = this->body + this->payload;
	};	

	void display() {
        std::cout << "version: " << this->version << std::endl;
        std::cout << "path: " << this->path << std::endl;
        std::cout << "headers: " << std::endl;
        for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); ++it) {
            std::cout << "key: \"" << it->first << "\" value: \"" << it->second << "\"" << std::endl;
        }
        std::cout << "method: " << this->method << std::endl;
        std::cout << "status_code: " << this->status_code << std::endl;
        // std::cout << "payload: " << this->payload << std::endl;
	};
};
