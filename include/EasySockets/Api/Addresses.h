#pragma once

#include <cstdint>
#include <string>

namespace es
{
    using Address = std::string;
    using Port = uint16_t;

    struct EndPoint
    {
        Address address;
        Port port;
    };
}
