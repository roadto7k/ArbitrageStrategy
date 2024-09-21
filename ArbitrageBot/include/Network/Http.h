#include <string>
#include <map>

struct HttpRequest {
    std::string url;
    std::map<std::string, std::string> headers;
    std::string body;
    std::string method; 
};


struct HttpResponse {
    int statusCode;
    std::string body;  
    std::map<std::string, std::string> headers;
    std::string errorMessage;
};