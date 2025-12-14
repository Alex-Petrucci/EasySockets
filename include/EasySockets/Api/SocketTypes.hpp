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
        IPV4,
        /// An Ipv6 address.
        IPV6,
        /// An IPv6 address which allows for conversions from IPv4.
        DUAL_STACK
    };

    /**
     * @ingroup CrossPlatformApi
     *
     * @brief Layer 4/transport layer protocol.
     */
    enum class Protocol
    {
        /// TCP protocol.
        TCP,
        /// UDP protocol.
        UDP
    };
}