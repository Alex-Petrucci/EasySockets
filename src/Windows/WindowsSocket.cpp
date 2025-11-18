#include <EasySockets/Windows/WindowsSocket.h>

#include <stdexcept>
#include <winsock2.h>
#include <ws2tcpip.h>


namespace es
{
    WindowsSocket::WindowsSocket()
        : m_socket{INVALID_SOCKET}
        , m_winsock_data{}
    {

    }

    WindowsSocket::~WindowsSocket()
    {
        closesocket(m_socket);
    }

    WindowsSocket::WindowsSocket(IpVersion ip_version, Protocol protocol)
        : WindowsSocket{}
    {
        switch (ip_version)
        {
        case IpVersion::ipv4:
            m_winsock_data.af = AF_INET;
            break;
        case IpVersion::ipv6:
            m_winsock_data.af = AF_INET6;
            break;
        }
        switch (protocol)
        {
        case Protocol::tcp:
            m_winsock_data.type = SOCK_STREAM;
            m_winsock_data.protocol = IPPROTO_TCP;
            break;
        case Protocol::udp:
            m_winsock_data.type = SOCK_DGRAM;
            m_winsock_data.protocol = IPPROTO_UDP;
            break;
        }

        m_socket = socket(m_winsock_data.af, m_winsock_data.type, m_winsock_data.protocol);

        if (m_socket == INVALID_SOCKET)
        {
            throw std::runtime_error("socket() failed: " + std::to_string(WSAGetLastError()));
        }
    }

    void WindowsSocket::bind_to(Address address, Port port)
    {
        sockaddr_in service;
        service.sin_family = m_winsock_data.af;
        service.sin_addr.s_addr = inet_addr(address);
        service.sin_port = htons(port);

        SOCKADDR* addr = reinterpret_cast<SOCKADDR*>(&service);

        if (bind(m_socket, addr, sizeof(service)) == SOCKET_ERROR)
        {
            throw std::runtime_error("bind() failed: " + std::to_string(WSAGetLastError()));
        }
    }

    void WindowsSocket::connect_to(Address address, Port port)
    {
        addrinfo hints{};
        hints.ai_family = m_winsock_data.af;
        hints.ai_socktype = m_winsock_data.type;
        hints.ai_protocol = m_winsock_data.protocol;

        addrinfo *result{nullptr};

        int res = getaddrinfo(address, std::to_string(port).c_str(), &hints, &result);
        if (res != 0)
            throw std::runtime_error("getaddrinfo() failed: " + std::to_string(res));

        if (connect(m_socket, result->ai_addr, static_cast<int>(result->ai_addrlen)) == SOCKET_ERROR)
        {
            throw std::runtime_error("connect() failed: " + std::to_string(WSAGetLastError()));
        }

        freeaddrinfo(result);
    }

    void WindowsSocket::listen_for_connections(int backlog)
    {
        if (listen(m_socket, backlog) == SOCKET_ERROR)
        {
            throw std::runtime_error("listen() failed: " + std::to_string(WSAGetLastError()));
        }
    }

    WindowsSocket WindowsSocket::accept_connection()
    {
        WindowsSocket accepting_socket{};
        accepting_socket.m_winsock_data = m_winsock_data;
        accepting_socket.m_socket = accept(m_socket, nullptr, nullptr);

        if (accepting_socket.m_socket == INVALID_SOCKET)
        {
            throw std::runtime_error("accept() failed: " + std::to_string(WSAGetLastError()));
        }

        return accepting_socket;
    }

    int WindowsSocket::receive_data(char* buffer, int buffer_size)
    {
        int bytes = recv(m_socket, buffer, buffer_size, 0);

        if (bytes < 0)
            throw std::runtime_error("recv() failed: " + std::to_string(WSAGetLastError()));

        return bytes;
    }

    int WindowsSocket::send_data(const char* buffer, int buffer_size)
    {
        int bytes = send(m_socket, buffer, buffer_size, 0);
        if (bytes < 0)
            throw std::runtime_error("send() failed: " + std::to_string(WSAGetLastError()));

        return bytes;
    }
}