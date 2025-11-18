#pragma once

#ifdef _WIN32

#include "Windows/WindowsSocket.h"
#include "Windows/WinsockContext.h"

namespace es
{
    using Socket = WindowsSocket;
    using Context = WinsockContext;
}

#else

#error EasySockets is only implemented for Windows currently

#endif