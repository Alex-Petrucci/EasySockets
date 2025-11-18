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