#ifndef WEBSOCKETCLIENTWEBSOCKETPP_H
#define WEBSOCKETCLIENTWEBSOCKETPP_H


#include "IWebSocketClient.h"
#include <websocketpp/client.hpp>
#include "websocketpp/config/asio_client.hpp"
#include <boost/asio/ssl/context.hpp>
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
    typedef websocketpp::client<websocketpp::config::asio_tls_client> Client;

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

    std::shared_ptr<boost::asio::ssl::context> onTlsInit(websocketpp::connection_hdl hdl);
};

#endif