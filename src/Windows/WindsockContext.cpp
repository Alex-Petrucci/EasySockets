#include <EasySockets/Windows/WinsockContext.h>

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

    WinsockContext::~WinsockContext()
    {
        WSACleanup();
    }

}