#include <gtest/gtest.h>
#include "WebSocketClientWebSocketPP.h"

TEST(WebSocketClientIntegrationTest, ConnectAndSendMessage) {
    WebSocketClientWebSocketPP client("ws://localhost:9000");

    client.connect();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    client.sendMessage("Hello, WebSocket!");

    std::this_thread::sleep_for(std::chrono::seconds(5));

    client.disconnect();
}