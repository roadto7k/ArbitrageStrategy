#ifndef WEBSOCKETCLIENTWEBSOCKETPP_H
#define WEBSOCKETCLIENTWEBSOCKETPP_H

#include "IWebSocketClient.h"
#include <websocketpp/client.hpp>
#include "websocketpp/config/asio_no_tls_client.hpp"
#include <vector>
#include <string>
#include <memory>
#include <mutex>

class WebSocketClientWebSocketPP : public IWebSocketClient {
public:
    WebSocketClientWebSocketPP(const std::string& uri);
    ~WebSocketClientWebSocketPP() override;

    void connect() override;
    void disconnect() override;
    void sendMessage(const std::string& message) override;

    void addObserver(IWebSocketObserver* observer) override;
    void removeObserver(IWebSocketObserver* observer) override;

private:
    typedef websocketpp::client<websocketpp::config::asio_client> Client;

    Client client;
    websocketpp::connection_hdl connectionHandle;
    std::string uri;
    bool isConnected;

    std::vector<IWebSocketObserver*> observers;
    std::mutex observerMutex;

    void onOpen(websocketpp::connection_hdl hdl);
    void onClose(websocketpp::connection_hdl hdl);
    void onMessage(websocketpp::connection_hdl hdl, Client::message_ptr msg);

    void notifyObservers(const std::string& message);
    void notifyConnectionClosed();
};

#endif