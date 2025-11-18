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
        WinsockContext(WinsockContext&& other) noexcept
            : m_wsa_data{other.m_wsa_data}
        {
            other.m_wsa_data = {};
        }
        WinsockContext& operator=(WinsockContext&& other) noexcept
        {
            m_wsa_data = other.m_wsa_data;
            other.m_wsa_data = {};
            return *this;
        }
        ~WinsockContext();

    private:
        WSADATA m_wsa_data;
    };
}