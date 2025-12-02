#include <EasySockets/Windows/WindowsSocket.hpp>

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
        if (m_socket != INVALID_SOCKET)
        {
            shutdown(m_socket, SD_SEND);
            closesocket(m_socket);
        }
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
        case IpVersion::dual_stack:
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

        if (ip_version == IpVersion::dual_stack)
        {
            constexpr int no = 0; // has to be an int, despite winsock taking a char*
            if (setsockopt(m_socket, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<const char *>(&no), sizeof(no)) == SOCKET_ERROR)
                throw std::runtime_error("setsockopt() failed: " + std::to_string(WSAGetLastError()));
        }
    }

    WindowsSocket::WindowsSocket(WindowsSocket&& other) noexcept
        : m_socket(other.m_socket)
        , m_winsock_data(other.m_winsock_data)
    {
        other.m_socket = INVALID_SOCKET;
        other.m_winsock_data = {};
    }

    WindowsSocket& WindowsSocket::operator=(WindowsSocket&& other) noexcept
    {
        m_socket = other.m_socket;
        m_winsock_data = other.m_winsock_data;
        other.m_socket = INVALID_SOCKET;
        other.m_winsock_data = {};

        return *this;
    }

    void WindowsSocket::bind_to(const EndPoint& end_point)
    {
        addrinfo* addr_info{resolve_address(end_point, AI_PASSIVE)};

        if (bind(m_socket, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)) == SOCKET_ERROR)
        {
            freeaddrinfo(addr_info);
            throw std::runtime_error("bind() failed: " + std::to_string(WSAGetLastError()));
        }

        freeaddrinfo(addr_info);
    }

    void WindowsSocket::connect_to(const EndPoint& end_point)
    {
        addrinfo* addr_info{resolve_address(end_point, 0)};

        if (connect(m_socket, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)) == SOCKET_ERROR)
        {
            freeaddrinfo(addr_info);
            throw std::runtime_error("connect() failed: " + std::to_string(WSAGetLastError()));
        }

        freeaddrinfo(addr_info);
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

        if (bytes == SOCKET_ERROR)
            throw std::runtime_error("recv() failed: " + std::to_string(WSAGetLastError()));

        return bytes;
    }

    int WindowsSocket::receive_data_from(char* buffer, int buffer_size, EndPoint& sender_end_point)
    {
        sockaddr_storage sender{};
        int sender_length = sizeof(sender);

        int bytes = recvfrom(m_socket, buffer, buffer_size, 0, reinterpret_cast<sockaddr*>(&sender), &sender_length);

        if (bytes == SOCKET_ERROR)
            throw std::runtime_error("recvfrom() failed: " + std::to_string(WSAGetLastError()));

        char ip_str[INET6_ADDRSTRLEN];
        void* addr_ptr{nullptr};

        if (sender.ss_family == AF_INET)
        {
            auto s = reinterpret_cast<sockaddr_in*>(&sender);
            addr_ptr = &s->sin_addr;
            sender_end_point.port = ntohs(s->sin_port);
        }
        if (sender.ss_family == AF_INET6)
        {
            auto s = reinterpret_cast<sockaddr_in6*>(&sender);
            addr_ptr = &s->sin6_addr;
            sender_end_point.port = ntohs(s->sin6_port);
        }

        inet_ntop(sender.ss_family, addr_ptr, ip_str, sizeof(ip_str));
        sender_end_point.address = ip_str;

        return bytes;
    }

    int WindowsSocket::send_data(const char* buffer, int buffer_size)
    {
        int bytes = send(m_socket, buffer, buffer_size, 0);

        if (bytes == SOCKET_ERROR)
            throw std::runtime_error("send() failed: " + std::to_string(WSAGetLastError()));

        return bytes;
    }

    int WindowsSocket::send_data_to(const char* buffer, int buffer_size, const EndPoint& end_point)
    {
        addrinfo* addr_info{resolve_address(end_point, 0)};

        int bytes = sendto(m_socket, buffer, buffer_size, 0, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen));

        freeaddrinfo(addr_info);

        if (bytes == SOCKET_ERROR)
            throw std::runtime_error("sendto() failed: " + std::to_string(WSAGetLastError()));

        return bytes;
    }

    addrinfo* WindowsSocket::resolve_address(const EndPoint& end_point, int flags)
    {
        addrinfo* result{nullptr};
        addrinfo hints{};
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = m_winsock_data.type;
        hints.ai_protocol = m_winsock_data.protocol;
        hints.ai_flags = flags;

        std::string port_string = std::to_string(end_point.port);
        int res = getaddrinfo(end_point.address.c_str(), port_string.c_str(), &hints, &result);
        if (res != 0)
            throw std::runtime_error("getaddrinfo() failed: " + std::to_string(res));

        return result;
    }
}