#pragma once

#include <csetjmp>
#include <vector>
#include <algorithm>
#include <iterator>
#include <optional>

extern jmp_buf g_restartJmp;

template <typename Container, typename T>
std::optional<size_t> indexOf(const Container& c, const T& value)
{
    auto it = std::find(std::begin(c), std::end(c), value);
    if (it == std::end(c))
        return std::nullopt;
    return static_cast<size_t>(std::distance(std::begin(c), it));
}