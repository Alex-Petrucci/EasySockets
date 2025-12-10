#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <EasySockets/Api/SocketTypes.hpp>
#include <EasySockets/Api/Addresses.hpp>
#include <EasySockets/Api/Documentation.hpp>

namespace es
{
    /// @ingroup PlatformSpecificApi
    ES_API_DOC(socket)
    /**
     * @brief An object which represents a connection between two applications.
     * @details On creation, type of ip addresses that the socket will use and the
     * layer 4/transport layer protocol is specified.
     * <br>From this, methods like <code>.bind_to(EndPoint end_point)</code>
     * and <code>.connect_to(EndPoint end_point)</code> to give the socket
     * an address and port so that it can start communicating with other applications.
     * <br>For connectionless protocols like UDP, data can be sent and received
     * without binding or connecting first.
     * <br>Can be moved from but not copied. Members are the same across all implementations.
     */
    class WindowsSocket
    {
    public:
        ES_API_DOC(socket_constructor)
        /**
         * @brief Constructor for Socket.
         * @param ip_version The version of ip addresses that the socket will use.
         * @param protocol The layer 4/transport layer protocol that the socket will use.
         */
        WindowsSocket(IpVersion ip_version, Protocol protocol);

        ~WindowsSocket();

        WindowsSocket(const WindowsSocket&) = delete;

        WindowsSocket& operator=(const WindowsSocket&) = delete;

        WindowsSocket(WindowsSocket&& other) noexcept;

        WindowsSocket& operator=(WindowsSocket&& other) noexcept;

        ES_API_DOC(socket_bind_to)
        /**
         * @brief Binds a socket to the given end point.
         * @param end_point The endpoint that the socket will use. <code>"0.0.0.0"</code>
         * will bind to all IPv4 interfaces, <code>"::0"</code>will bind to all IPv6 interfaces
         * and <code>"::"</code>" will bind to all interfaces.
         */
        void bind_to(const EndPoint& end_point);

        ES_API_DOC(socket_connect_to)
        /**
         * @brief Connects a socket to the given end point.
         * @param end_point The endpoint that the socket will use.
         */
        void connect_to(const EndPoint& end_point);

        ES_API_DOC(socket_listen_for_connections)
        /**
         * @brief Makes the socket start listening for incoming connections.
         * @details Must be bound with <code>.bind(EndPoint end_point)</code>
         * before being called.
         * @param backlog The amount of connections that can be waiting
         * to be accepted at a time. If the amount of connections exceeds
         * the backlog, then new connections will be dropped.
         * <br>Connections can be accepted with <code>.accept_connection</code>.
         */
        void listen_for_connections(int backlog);

        ES_API_DOC(socket_accept_connection)
        /**
         * @brief Accepts incoming connections.
         * @details <code>.listen(int backlog)</code>must be called first.
         * @return New socket connected to the socket that was attempting
         * to connect.
         */
        WindowsSocket accept_connection();

        ES_API_DOC(socket_receive_data)
        /**
         * @brief Waits to receive data from the connected end point.
         * @details Must be connected to another end point.
         * @param buffer The buffer which data received will be placed into.
         * @param buffer_size The size of the buffer provided.
         * @return The amount of data in bytes received.
         */
        int64_t receive_data(char* buffer, int buffer_size);

        ES_API_DOC(socket_receive_data_from)
        /**
         * @brief Waits to receive data from any endpoint.
         * @details Can only be used with connectionless protocols like UDP.
         * @param buffer The buffer which data received will be placed into.
         * @param buffer_size The size of the buffer provided.
         * @param sender_end_point An out parameter will give back the end point
         * of the sender.
         * @return The amount of data in bytes received.
         */
        int64_t receive_data_from(char* buffer, int buffer_size, EndPoint& sender_end_point);

        ES_API_DOC(socket_send_data)
        /**
         * @brief Sends data to the connected end point.
         * @details Must be connected to another end point.
         * @param buffer The buffer which data received will be placed into.
         * @param buffer_size The size of the buffer provided.
         * @return The amount of data in bytes sent.
         */
        int64_t send_data(const char* buffer, int buffer_size);

        ES_API_DOC(socket_send_data_to)
        /**
         * @brief Sends data to the provided end point.
         * @details Must be connected to another end point.
         * @param buffer The buffer which data received will be placed into.
         * @param buffer_size The size of the buffer provided.
         * @param end_point The endpoint that the data will be sent to.
         * @return The amount of data in bytes sent.
         */
        int64_t send_data_to(const char* buffer, int buffer_size, const EndPoint& end_point);

    private:
        // data that winsock needs for the socket
        struct WinsockData
        {
            int af;
            int type;
            int protocol;
        };

        // only for internal use when creating a socket for accepting
        WindowsSocket();

        // must free with freeaddrinfo
        addrinfo* resolve_address(const EndPoint& end_point, int flags);

        SOCKET m_socket;
        WinsockData m_winsock_data;
    };
}