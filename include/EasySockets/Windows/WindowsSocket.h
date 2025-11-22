#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
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
        WindowsSocket(WindowsSocket&& other) noexcept;
        WindowsSocket& operator=(WindowsSocket&& other) noexcept;

        void bind_to(const EndPoint& end_point);
        void connect_to(const EndPoint& end_point);
        void listen_for_connections(int backlog);
        WindowsSocket accept_connection();
        int receive_data(char* buffer, int buffer_size);
        int receive_data_from(char* buffer, int buffer_size, EndPoint& sender_end_point);
        int send_data(const char* buffer, int buffer_size);
        int send_data_to(const char* buffer, int buffer_size, const EndPoint& end_point);

    private:
        struct WinsockData
        {
            int af;
            int type;
            int protocol;
        };

        WindowsSocket();

        // must free with freeaddrinfo
        addrinfo* resolve_address(const EndPoint& end_point, int flags);

        SOCKET m_socket;
        WinsockData m_winsock_data;
    };
}