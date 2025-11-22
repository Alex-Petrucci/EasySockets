#pragma once

namespace es
{
    enum class IpVersion
    {
        ipv4,
        ipv6,
        unspecified
    };

    enum class Protocol
    {
        tcp,
        udp
    };
}