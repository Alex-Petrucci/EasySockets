#pragma once

#include <winsock2.h>


namespace es
{
    class WinsockContext
    {
    public:
        WinsockContext();
        WinsockContext(const WinsockContext&) = delete;
        WinsockContext& operator=(const WinsockContext&) = delete;
        WinsockContext(WinsockContext&& other) noexcept;
        WinsockContext& operator=(WinsockContext&& other) noexcept;

        ~WinsockContext();

    private:
        WSADATA m_wsa_data;
    };
}