#pragma once

#include <EasySockets/Api/Documentation.hpp>


namespace es
{
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
    class PosixContext
    {
    public:
        PosixContext() = default;
        PosixContext(const PosixContext&) = delete;
        PosixContext& operator=(const PosixContext&) = delete;
        PosixContext(PosixContext&& other) noexcept = default;
        PosixContext& operator=(PosixContext&& other) noexcept;
        ~PosixContext() = default;
    };
}