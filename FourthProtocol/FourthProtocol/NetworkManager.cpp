#include "NetworkManager.h"
#include <iostream>

NetworkManager::NetworkManager()
    : m_isHost(false)
    , m_connectionState(ConnectionState::Idle)
{
}

NetworkManager::~NetworkManager()
{
    disconnect();
}

void NetworkManager::setNonBlocking(bool t_nonBlocking)
{
    m_socket.setBlocking(!t_nonBlocking);
}

bool NetworkManager::startHostAsync(unsigned short t_port)
{
    std::cout << "[NetworkManager] Starting host on port " << t_port << "\n";

    m_isHost = true;

    // Bind listener to port
    if (m_listener.listen(t_port) != sf::Socket::Status::Done) {
        std::cout << "[NetworkManager] ERROR: Could not bind to port " << t_port << "\n";
        m_connectionState = ConnectionState::Failed;
        return false;
    }

    // Make listener non-blocking so accept() doesn't freeze the game
    m_listener.setBlocking(false);

    std::cout << "[NetworkManager] Listening on port " << t_port << "...\n";
    m_connectionState = ConnectionState::Listening;

    return true;
}

bool NetworkManager::connectAsClientAsync(const std::string& t_ip, unsigned short t_port)
{
    std::cout << "[NetworkManager] Connecting to " << t_ip << ":" << t_port << "\n";

    m_isHost = false;

    // Resolve IP address
    auto addressOpt = sf::IpAddress::resolve(t_ip);
    if (!addressOpt.has_value())
    {
        std::cout << "[NetworkManager] ERROR: Invalid IP address: " << t_ip << "\n";
        m_connectionState = ConnectionState::Failed;
        return false;
    }

    sf::IpAddress address = addressOpt.value();

    // Set socket non-blocking BEFORE connecting
    m_socket.setBlocking(false);

    m_connectionState = ConnectionState::Connecting;

    // Attempt connection (returns immediately in non-blocking mode)
    sf::Socket::Status status = m_socket.connect(address, t_port);

    if (status == sf::Socket::Status::Done) 
    {
        // Connected immediately (rare, but possible on localhost)
        std::cout << "[NetworkManager] Connected immediately!\n";
        m_connectionState = ConnectionState::Connected;
        return true;
    }
    else if (status == sf::Socket::Status::NotReady)
    {
        // Connection in progress (normal for non-blocking)
        std::cout << "[NetworkManager] Connection in progress...\n";
        return true;  // Not failed, just pending
    }
    else 
    {
        // Connection failed
        std::cout << "[NetworkManager] ERROR: Connection failed\n";
        m_connectionState = ConnectionState::Failed;
        return false;
    }
}

void NetworkManager::updateConnection()
{
    if (m_connectionState == ConnectionState::Listening) 
    {
        // Host: Check if client connected
        sf::Socket::Status status = m_listener.accept(m_socket);

        if (status == sf::Socket::Status::Done) 
        {
            std::cout << "[NetworkManager] Client connected!\n";
            m_socket.setBlocking(false);  // Make socket non-blocking
            m_connectionState = ConnectionState::Connected;
        }
        else if (status == sf::Socket::Status::Error)
        {
            std::cout << "[NetworkManager] ERROR: Accept failed\n";
            m_connectionState = ConnectionState::Failed;
        }
        // Status::NotReady means no connection yet (normal)
    }
    else if (m_connectionState == ConnectionState::Connecting)
    {
        // Client: Check if connection completed
        // Try sending a test packet to check if connected
        sf::Packet testPacket;
        testPacket << std::string("PING");

        sf::Socket::Status status = m_socket.send(testPacket);

        if (status == sf::Socket::Status::Done)
        {
            std::cout << "[NetworkManager] Connection established!\n";
            m_connectionState = ConnectionState::Connected;
        }
        else if (status == sf::Socket::Status::Error || status == sf::Socket::Status::Disconnected) 
        {
            std::cout << "[NetworkManager] Connection failed\n";
            m_connectionState = ConnectionState::Failed;
        }
        // Status::NotReady means still connecting
    }
}

bool NetworkManager::sendMessage(const std::string& message)
{
    if (m_connectionState != ConnectionState::Connected)
    {
        std::cout << "[NetworkManager] ERROR: Not connected, cannot send\n";
        return false;
    }

    sf::Packet packet;
    packet << message;

    sf::Socket::Status status = m_socket.send(packet);

    if (status == sf::Socket::Status::Done) 
    {
        std::cout << "[NetworkManager] Sent: " << message << "\n";
        return true;
    }
    else if (status == sf::Socket::Status::Disconnected)
    {
        std::cout << "[NetworkManager] Connection lost while sending\n";
        m_connectionState = ConnectionState::Failed;
        return false;
    }
    else
    {
        std::cout << "[NetworkManager] ERROR: Failed to send message\n";
        return false;
    }
}

std::optional<std::string> NetworkManager::receiveMessage()
{
    if (m_connectionState != ConnectionState::Connected)
    {
        return std::nullopt;
    }

    sf::Packet packet;
    sf::Socket::Status status = m_socket.receive(packet);

    if (status == sf::Socket::Status::Done) 
    {
        std::string message;
        packet >> message;
        std::cout << "[NetworkManager] Received: " << message << "\n";
        return message;
    }
    else if (status == sf::Socket::Status::Disconnected) 
    {
        std::cout << "[NetworkManager] Connection lost\n";
        m_connectionState = ConnectionState::Failed;
        return std::nullopt;
    }
    else if (status == sf::Socket::Status::NotReady)
    {
        // No data available (normal in non-blocking mode)
        return std::nullopt;
    }
    else {
        // Error
        std::cout << "[NetworkManager] ERROR: Socket error\n";
        return std::nullopt;
    }
}

void NetworkManager::disconnect()
{
    if (m_connectionState != ConnectionState::Idle) 
    {
        std::cout << "[NetworkManager] Disconnecting...\n";
        m_socket.disconnect();
        m_connectionState = ConnectionState::Idle;
    }

    if (m_isHost) 
    {
        m_listener.close();
    }
}