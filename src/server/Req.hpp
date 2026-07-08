class Req : public ARe {
private:
    std::string                         reqMethod;
    std::string                         reqPath;
public:
    // OCF
    Req();
    Req( const std::string &newHeaders);
    Req( const Req &other );
    Req &operator=( const Req &other );
    ~Req();


	// GETTERS
    std::string getMethod();
    std::string getPath();

    void display();

};
