#pragma once
#include <SFML/Network.hpp>
#include <string>
#include <optional>

enum class ConnectionState {
    Idle,           // Not connected
    Listening,      // Host waiting for connection
    Connecting,     // Client attempting to connect
    Connected,      // Fully connected
    Failed          // Connection failed
};

class NetworkManager {
private:
    bool m_isHost;
    ConnectionState m_connectionState;

    sf::TcpSocket m_socket;
    sf::TcpListener m_listener;

    void setNonBlocking(bool nonBlocking);

public:
    NetworkManager();
    ~NetworkManager();

    // Setup (non-blocking versions)
    bool startHostAsync(unsigned short port = 53000);
    bool connectAsClientAsync(const std::string& ip, unsigned short port = 53000);

    // Update connection status (call every frame)
    void updateConnection();

    // Status
    ConnectionState getConnectionState() const { return m_connectionState; }
    bool isConnected() const { return m_connectionState == ConnectionState::Connected; }
    bool isHost() const { return m_isHost; }

    // Sending
    bool sendMessage(const std::string& message);

    // Receiving (non-blocking)
    std::optional<std::string> receiveMessage();

    // Cleanup
    void disconnect();
};