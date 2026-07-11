class ARe {
protected:
	std::string							body;
    std::string                         version;
	std::string							path;
    std::map<std::string, std::string>  headers;
	// Only the content of the message, not to confuse with
	// the body that also contains method, headers, etc
    std::string                         content;
    int			                        contentLen;
	virtual void						display() = 0;
public:
	// ####### OCF
	ARe() : contentLen(0) {};
	ARe( const ARe &other ) { *this = other; };
	ARe &operator=( const ARe &other ) {
		if (this != &other) {
			this->body = other.body;
			this->version = other.version;
			this->path = other.path;
			this->headers = other.headers;
			this->content = other.content;
			this->contentLen = other.contentLen;
		}
		return (*this);
	};
	virtual ~ARe() {};

	// ####### GETTERS
	std::string getBody() { return (this->body); }
    std::string getVersion() { return (this->version); }
    std::string getPath() { return (this->path); }
    std::map<std::string, std::string> getHeaders() { return (this->headers); }
    const char *getHeader( std::string strKey ) {
		std::map<std::string, std::string>::iterator it = headers.find(strKey);
		if (it == this->headers.end())
			return (NULL);
        return (it->second.c_str());
    }
    std::string getContent() { return (this->content); }
    int	getContentLen() { return (this->contentLen); }

	// ####### SETTERS
	void setBody( std::string newBody ) { this->body = newBody; }
	void setVersion( std::string newVersion ) { this->version = newVersion; }
	void setPath( std::string newPath ) { this->path = newPath; }
	void addHeader(std::string strKey, std::string strValue) {
		headers.insert(std::pair<std::string, std::string>(strKey, strValue));
	};
	void addContent(const std::string &newContent) {
		this->content = this->content + newContent;
		this->contentLen = content.length();
	};

	// ####### Display
	void ADisplay() {
        std::cout << "version: " << this->version << std::endl;
        std::cout << "path: " << this->path << std::endl;
        std::cout << "headers: " << std::endl;
        for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); ++it) {
            std::cout << "key: \"" << it->first << "\" value: \"" << it->second << "\"" << std::endl;
        }
        std::cout << "content: " << this->content << std::endl;
	};
};

#include "Res.hpp"
#include "Req.hpp"
