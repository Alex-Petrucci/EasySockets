#pragma once

#include "Api/SocketApi.hpp"
#include "Api/ContextApi.hpp"
#include "Api/Documentation.hpp"

#if defined(_WIN32)

    #include "Windows/WindowsSocket.hpp"
    #include "Windows/WinsockContext.hpp"

    #define SOCKET_TYPE WindowsSocket
    #define CONTEXT_TYPE WinsockContext

#elif defined(__unix__) || defined(__unix) || defined(__APPLE__)

    #include "Posix/PosixSocket.hpp"
    #include "Posix/PosixContext.hpp"

    #define SOCKET_TYPE PosixSocket
    #define CONTEXT_TYPE PosixContext

#else

    #error EasySockets does not support this platform

#endif

namespace es
{
    ES_API_DOC(socket)
    /**
     * An object which represents a connection between two applications.
     * <br>On creation, type of ip addresses that the socket will use and the
     * layer 4/transport layer protocol is specified.
     * <br>From this, methods like <code>.bind_to(EndPoint end_point)</code>
     * and <code>.connect_to(EndPoint end_point)</code> to give the socket
     * an address and port so that it can start communicating with other applications.
     * <br>For connectionless protocols like UDP, data can be sent and received
     * without binding or connecting first.
     * <br>Can be moved from but not copied.
     */
    using Socket = SOCKET_TYPE;

    ES_API_DOC(context)
    /**
     * An object which is required to exist for es::Sockets to be used.
     * This ensures that any underlying setup and cleanup code is called.
     * <br>For example:
     * <code>
     * es::Context context{}; // must be created before any sockets are created
     * es::Socket socket{es::IpVersion::ipv4, es::Protocol::tcp};
     * socket.bind({"0.0.0.0", 8080});
     * </code>
     * <br>Can be moved from but not copied.
     */
    using Context = CONTEXT_TYPE;
}

static_assert(es::SocketApi<es::Socket>,
              "Socket API for current platform is invalid. This problem is the library's fault, not the end user's.");
static_assert(es::ContextApi<es::Context>,
              "Context API for current platform is invalid. This problem is the library's fault, not the end user's.");
