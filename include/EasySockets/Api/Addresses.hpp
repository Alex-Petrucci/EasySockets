#pragma once

#include <cstdint>
#include <string>

namespace es
{
    /**
     * String IP address.
     */
    using Address = std::string;

    /**
     * 16-bit port number.
     */
    using Port = uint16_t;

    /**
     * Represents the address + port that identifies a connection.
     * Initialized by <code>{Address, Port}</code>, e.g. <code>{"192.168.0.1", 443}</code>.
     */
    struct EndPoint
    {
        Address address;
        Port port;
    };
}
