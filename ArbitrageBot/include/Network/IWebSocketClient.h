#ifndef IWEBSOCKETCLIENT_H
#define IWEBSOCKETCLIENT_H

#include <string>

class IWebSocketObserver {
public:
    virtual void onMessageReceived(const std::string& message) = 0;
    virtual void onConnectionClosed() = 0;
    virtual ~IWebSocketObserver() = default;
};

class IWebSocketClient {
public:
    virtual ~IWebSocketClient() = default;

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void sendMessage(const std::string& message) = 0;

    virtual void addObserver(IWebSocketObserver* observer) = 0;
    virtual void removeObserver(IWebSocketObserver* observer) = 0;
};

#endif