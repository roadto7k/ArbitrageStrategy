#include "Http.h"
#include <curl/curl.h>

class HttpClient {
public:
    virtual HttpResponse sendRequest(const HttpRequest& request) = 0;

    virtual void setTimeout(int milliseconds) = 0;

    virtual void setRetryPolicy(int retries) = 0;

    virtual ~HttpClient() = default;
};

class CurlHttpClient : public HttpClient {
public:
    CurlHttpClient();
    ~CurlHttpClient();

    HttpResponse sendRequest(const HttpRequest& request) override;

    void setTimeout(int milliseconds) override;
    void setRetryPolicy(int retries) override;

private:
    CURL* curlHandle;
    int timeout;
    int retryCount;

    HttpResponse processCurlResponse(CURLcode curlCode, const std::string& responseData);
};