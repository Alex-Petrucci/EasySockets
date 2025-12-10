#pragma once

#include <cstdint>
#include <string>

namespace es
{
    /**
     * @ingroup CrossPlatformApi
     *
     * @brief String IP address.
     */
    using Address = std::string;

    /**
     * @ingroup CrossPlatformApi
     *
     * @brief 16-bit port number.
     */
    using Port = uint16_t;

    /**
     * @ingroup CrossPlatformApi
     *
     * @brief Represents the address + port that identifies a connection.
     * Initialized by <code>{Address, Port}</code>, e.g. <code>{"192.168.0.1", 443}</code>.
     */
    struct EndPoint
    {
        Address address;
        Port port;
    };
}
