#include <EasySockets/Posix/PosixSocket.hpp>

#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


namespace es
{
    PosixSocket::PosixSocket()
        : m_socket{-1}
        , m_socket_data{}
    {
    }

    PosixSocket::PosixSocket(IpVersion ip_version, Protocol protocol)
        : PosixSocket{}
    {
        switch (ip_version)
        {
        case IpVersion::IPV4:
            m_socket_data.af = AF_INET;
            break;
        case IpVersion::IPV6:
        case IpVersion::DUAL_STACK:
            m_socket_data.af = AF_INET6;
            break;
        }

        switch (protocol)
        {
        case Protocol::TCP:
            m_socket_data.type = SOCK_STREAM;
            m_socket_data.protocol = IPPROTO_TCP;
            break;
        case Protocol::UDP:
            m_socket_data.type = SOCK_DGRAM;
            m_socket_data.protocol = IPPROTO_UDP;
            break;
        }

        m_socket = socket(m_socket_data.af, m_socket_data.type, m_socket_data.protocol);
        if (m_socket < 0)
            throw std::runtime_error("socket() failed: " + std::string{strerror(errno)});

        int one = 1;
        if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)))
            throw std::runtime_error("setsockopt() failed: " + std::string{strerror(errno)});
        if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one)))
            throw std::runtime_error("setsockopt() failed: " + std::string{strerror(errno)});

        if (m_socket_data.af == AF_INET6)
        {
            int not_dual_stack = ip_version == IpVersion::DUAL_STACK ? 0 : 1;
            if (setsockopt(m_socket, IPPROTO_IPV6, IPV6_V6ONLY, &not_dual_stack, sizeof(not_dual_stack)) < 0)
                throw std::runtime_error("setsockopt() failed: " + std::string{strerror(errno)});
        }
    }

    PosixSocket::~PosixSocket()
    {
        shutdown(m_socket, SHUT_WR);
        close(m_socket);
    }

    PosixSocket::PosixSocket(PosixSocket&& other) noexcept
        : m_socket{other.m_socket}
        , m_socket_data{other.m_socket_data}
    {
        other.m_socket = -1;
        other.m_socket_data = {};
    }

    PosixSocket& PosixSocket::operator=(PosixSocket&& other) noexcept
    {
        m_socket = other.m_socket;
        other.m_socket = -1;
        return *this;
    }

    void PosixSocket::bind_to(const EndPoint& end_point)
    {
        addrinfo* addr_info{resolve_address(end_point)};

        bool success = false;
        for (addrinfo* info = addr_info; info != nullptr; info = info->ai_next)
        {
            if (bind(m_socket, info->ai_addr, info->ai_addrlen) == 0)
            {
                success = true;
                break;
            }
        }

        freeaddrinfo(addr_info);

        if (!success)
            throw std::runtime_error("bind() failed: " + std::string{strerror(errno)});
    }

    void PosixSocket::connect_to(const EndPoint& end_point)
    {
        addrinfo* addr_info{resolve_address(end_point)};

        bool success = false;
        for (addrinfo* info = addr_info; info != nullptr; info = info->ai_next)
        {
            if (connect(m_socket, info->ai_addr, info->ai_addrlen) == 0)
            {
                success = true;
                break;
            }
        }

        freeaddrinfo(addr_info);

        if (!success)
            throw std::runtime_error("connect() failed: " + std::string{strerror(errno)});
    }

    void PosixSocket::listen_for_connections(int backlog)
    {
        if (listen(m_socket, backlog) < 0)
            throw std::runtime_error("listen() failed: " + std::string{strerror(errno)});
    }

    PosixSocket PosixSocket::accept_connection()
    {
        int accepting_socket = accept(m_socket, nullptr, nullptr);

        if (accepting_socket < 0)
            throw std::runtime_error("accept() failed: " + std::string{strerror(errno)});

        PosixSocket return_socket{};
        return_socket.m_socket = accepting_socket;
        return_socket.m_socket_data = m_socket_data;

        return return_socket;
    }

    int64_t PosixSocket::receive_data(char* buffer, int buffer_size)
    {
        ssize_t bytes = recv(m_socket, buffer, buffer_size, 0);
        if (bytes < 0)
            throw std::runtime_error("recv() failed: " + std::string{strerror(errno)});

        return bytes;
    }

    int64_t PosixSocket::receive_data_from(char* buffer, int buffer_size, EndPoint& sender_end_point)
    {
        sockaddr_storage sender{};
        socklen_t sender_length{sizeof(sender)};

        ssize_t bytes = recvfrom(m_socket, buffer, buffer_size, 0, reinterpret_cast<sockaddr*>(&sender), &sender_length);
        if (bytes < 0)
            throw std::runtime_error("recvfrom() failed: " + std::string{strerror(errno)});

        char host[NI_MAXHOST]{};
        char service[NI_MAXSERV]{};

        int error = getnameinfo(reinterpret_cast<sockaddr*>(&sender), sender_length,
            host, sizeof(host),
            service, sizeof(service),
            NI_NUMERICHOST | NI_NUMERICSERV);

        if (error != 0)
            throw std::runtime_error("getnameinfo() failed: " + std::string{gai_strerror(error)});

        sender_end_point.address = std::string{host};
        sender_end_point.port = std::stoi(service);

        return bytes;
    }

    int64_t PosixSocket::send_data(const char* buffer, int buffer_size)
    {
        ssize_t bytes = send(m_socket, buffer, buffer_size, 0);
        if (bytes < 0)
            throw std::runtime_error("send() failed: " + std::string{strerror(errno)});

        return bytes;
    }

    int64_t PosixSocket::send_data_to(const char* buffer, int buffer_size, const EndPoint& end_point)
    {
        addrinfo* addr_info{resolve_address(end_point)};

        ssize_t bytes = -1;
        for (addrinfo* info = addr_info; info != nullptr; info = info->ai_next)
        {
            bytes = sendto(m_socket, buffer, buffer_size, 0, info->ai_addr, info->ai_addrlen);
            if (bytes < 0)
            {
                break;
            }
        }

        freeaddrinfo(addr_info);

        if (bytes < 0)
            throw std::runtime_error("sendto() failed: " + std::string{strerror(errno)});

        return buffer_size;
    }


    addrinfo* PosixSocket::resolve_address(const EndPoint& end_point)
    {
        addrinfo hints{};
        addrinfo* result{nullptr};

        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = m_socket_data.type;
        hints.ai_protocol = m_socket_data.protocol;

        int error = getaddrinfo(end_point.address.c_str(), std::to_string(end_point.port).c_str(), &hints, &result);
        if (error != 0)
            throw std::runtime_error("getaddrinfo() failed: " + std::string{gai_strerror(error)});

        return result;
    }
}
