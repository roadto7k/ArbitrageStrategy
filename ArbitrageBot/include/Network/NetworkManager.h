#include <memory>
#include <string>
#include <iostream>
#include "IWebSocketClient.h"
#include "HttpClient.h"

class NetworkManager {
public:
    NetworkManager(std::unique_ptr<HttpClient> httpClient, std::unique_ptr<IWebSocketClient> webSocketClient);

    HttpResponse makeHttpRequest(const HttpRequest& request);

    void connectWebSocket(const std::string& symbol, IWebSocketObserver* observer);

    void setHttpTimeout(int milliseconds);
    void setHttpRetryPolicy(int retries);
    void setWebSocketReconnectPolicy(bool enableAutoReconnect);

private:
    std::unique_ptr<HttpClient> httpClient; 
    std::unique_ptr<IWebSocketClient> webSocketClient;
};