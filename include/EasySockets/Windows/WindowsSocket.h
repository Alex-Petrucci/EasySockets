#pragma once

#include <winsock2.h>
#include <EasySockets/Api/SocketTypes.h>
#include <EasySockets/Api/Addresses.h>

namespace es
{
    class WindowsSocket
    {
    public:
        WindowsSocket(IpVersion ip_version, Protocol protocol);
        ~WindowsSocket();
        WindowsSocket(const WindowsSocket&) = delete;
        WindowsSocket& operator=(const WindowsSocket&) = delete;
        WindowsSocket(WindowsSocket&& other) noexcept
            : m_socket(other.m_socket)
            , m_winsock_data(other.m_winsock_data)
        {
            other.m_socket = INVALID_SOCKET;
            other.m_winsock_data = {};
        }
        WindowsSocket& operator=(WindowsSocket&& other) noexcept
        {
            m_socket = other.m_socket;
            m_winsock_data = other.m_winsock_data;
            other.m_socket = INVALID_SOCKET;
            other.m_winsock_data = {};

            return *this;
        }

        void bind_to(Address address, Port port);
        void connect_to(Address address, Port port);
        void listen_for_connections(int backlog);
        WindowsSocket accept_connection();
        int receive_data(char* buffer, int buffer_size);
        int send_data(const char* buffer, int buffer_size);

    private:
        struct WinsockData
        {
            int af;
            int type;
            int protocol;
        };

        WindowsSocket();

        SOCKET m_socket;
        WinsockData m_winsock_data;
    };
}