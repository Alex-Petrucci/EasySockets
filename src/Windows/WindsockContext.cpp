#include <EasySockets/Windows/WinsockContext.hpp>

#include <stdexcept>


namespace es
{
    WinsockContext::WinsockContext()
        : m_wsa_data{}
    {
        int wsa_error = WSAStartup(MAKEWORD(2, 2), &m_wsa_data);

        if (wsa_error != 0)
            throw std::runtime_error("Can't start winsock");
    }

    WinsockContext::WinsockContext(WinsockContext&& other) noexcept
        : m_wsa_data{other.m_wsa_data}
    {
        other.m_wsa_data = {};
    }

    WinsockContext& WinsockContext::operator=(WinsockContext&& other) noexcept
    {
        m_wsa_data = other.m_wsa_data;
        other.m_wsa_data = {};
        return *this;
    }

    WinsockContext::~WinsockContext()
    {
        WSACleanup();
    }

}