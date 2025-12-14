#pragma once

#include <winsock2.h>
#include <EasySockets/Api/Documentation.hpp>

namespace es
{
    /// @ingroup PlatformSpecificApi
    ES_API_DOC(context)
    /**
     * @brief An object which is required to exist for es::Sockets to be used.
     * @details This ensures that any underlying setup and cleanup code is called.
     * <br>For example:
     * <code>
     * es::Context context{}; // must be created before any sockets are created
     * es::Socket socket{es::IpVersion::ipv4, es::Protocol::tcp};
     * socket.bind({"0.0.0.0", 8080});
     * </code>
     * <br>Can be moved from but not copied. Members are the same across all implementations.
     */
    class WinsockContext
    {
    public:
        WinsockContext();
        WinsockContext(const WinsockContext&) = delete;
        WinsockContext& operator=(const WinsockContext&) = delete;
        WinsockContext(WinsockContext&& other) noexcept;
        WinsockContext& operator=(WinsockContext&& other) noexcept;
        ~WinsockContext();

        void shutdown();

    private:
        WSADATA m_wsa_data;
    };
}