#pragma once

namespace es
{
    enum class IpVersion
    {
        ipv4,
        ipv6,
        dual_stack
    };

    enum class Protocol
    {
        tcp,
        udp
    };
}