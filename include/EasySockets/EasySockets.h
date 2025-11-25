#pragma once

#include "Api/SocketApi.h"
#include "Api/ContextApi.h"

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

static_assert(es::SocketApi<es::Socket>,
    "Socket API for current platform is invalid. This problem is the library's fault, not the end user's.");
static_assert(es::ContextApi<es::Context>,
    "Context API for current platform is invalid. This problem is the libarys's fault, not the end user's.");
