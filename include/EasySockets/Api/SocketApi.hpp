#pragma once

#include "Addresses.hpp"
#include "SocketTypes.hpp"

namespace es
{
    template<typename T>
    concept SocketApi = requires(T t, EndPoint end_point, char* buffer, int len, IpVersion ip_version)
    {
        { t.bind_to(end_point) };
        { t.connect_to(end_point) };
        { t.listen_for_connections(len) };
        { t.accept_connection() }                           -> std::same_as<T>;
        { t.receive_data(buffer, len) }                     -> std::same_as<int64_t>;
        { t.receive_data_from(buffer, len, end_point) }     -> std::same_as<int64_t>;
        { t.send_data(buffer, len) }                        -> std::same_as<int64_t>;
        { t.send_data_to(buffer, len, end_point) }          -> std::same_as<int64_t>;
        { t.close() };
        { T::make_connected_tcp(end_point) }                -> std::same_as<T>;
        { T::make_connected_udp(end_point) }                -> std::same_as<T>;
        { T::make_bound_tcp(end_point.port, ip_version) }   -> std::same_as<T>;
        { T::make_bound_tcp(end_point.port) }               -> std::same_as<T>;
        { T::make_bound_udp(end_point.port, ip_version) }   -> std::same_as<T>;
        { T::make_bound_udp(end_point.port) }               -> std::same_as<T>;
    }
    && std::constructible_from<T, IpVersion, Protocol>
    && !std::copyable<T>
    && std::movable<T>;
}