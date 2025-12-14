#pragma once

#include <concepts>

namespace es
{
    template<typename T>
    concept ContextApi = requires(T t)
    {
        { t.shutdown() };
    } &&
    std::is_default_constructible_v<T>
    && !std::copyable<T>
    && std::movable<T>;
}