class Res : public ARe {
private:
    int									resStatusCode;
public:
	Res();
	Res( std::string path );
    Res( const Res &other );
    Res &operator=( const Res &other );
    ~Res();

	std::string toString();

	// ####### Getters
	int getStatusCode();

    void display();
	bool getFileContent( std::string &path );

};
