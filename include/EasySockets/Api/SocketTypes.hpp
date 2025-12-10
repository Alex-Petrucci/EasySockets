#pragma once

namespace es
{
    /**
     * @ingroup CrossPlatformApi
     *
     * @brief Version of IP address.
     */
    enum class IpVersion
    {
        /// An IPv4 address.
        ipv4,
        /// An Ipv6 address.
        ipv6,
        /// An IPv6 address which allows for conversions from IPv4.
        dual_stack
    };

    /**
     * @ingroup CrossPlatformApi
     *
     * @brief Layer 4/transport layer protocol.
     */
    enum class Protocol
    {
        /// TCP protocol.
        tcp,
        /// UDP protocol.
        udp
    };
}