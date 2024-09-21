#include "Http.h"

class HttpClient {
public:

    virtual HttpResponse sendGetRequest(const std::string& url, const std::map<std::string, std::string>& headers) = 0;
    virtual HttpResponse sendPostRequest(const std::string& url, const std::map<std::string, std::string>& headers, const std::string& body) = 0;

    virtual ~HttpClient() = default;
};

class CurlHttpClient : public HttpClient {
public:
    CurlHttpClient();  // Initialisation de libcurl
    ~CurlHttpClient(); // Nettoyage de libcurl

    HttpResponse sendGetRequest(const std::string& url, const std::map<std::string, std::string>& headers) override;
    HttpResponse sendPostRequest(const std::string& url, const std::map<std::string, std::string>& headers, const std::string& body) override;

private:
    // Gestion interne de libcurl (par exemple CURL handle)
};