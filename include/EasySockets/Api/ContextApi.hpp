#pragma once

#include <concepts>

namespace es
{
    template<typename T>
    concept ContextApi = std::is_default_constructible_v<T>
        && !std::copyable<T>
        && std::movable<T>;
}