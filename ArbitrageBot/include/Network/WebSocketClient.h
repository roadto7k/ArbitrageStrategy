#include <string>
#include <vector>

class WebSocketClient {
public:
    WebSocketClient(const std::string& url);
    ~WebSocketClient();

    void connect();
    void disconnect();

    void addObserver(IWebSocketObserver* observer);

    void sendMessage(const std::string& message);

private:
    std::string url; 
    std::vector<IWebSocketObserver*> observers;

    void notifyObservers(const std::string& message);

    void onMessageReceived(const std::string& message);
    void onConnectionClosed();
};

class IWebSocketObserver {
public:
    virtual void onMessageReceived(const std::string& message) = 0;
    virtual ~IWebSocketObserver() = default;
};