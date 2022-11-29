#ifndef ADBLOCK_INDEX_UTILITY_HPP
#define ADBLOCK_INDEX_UTILITY_HPP

#include "namespace.hpp"

#include "core/string_range.hpp"

#include <algorithm>

#include <stream9/strings/trim.hpp>

namespace adblock {

inline StringRange
firstToken(StringRange pattern)
{
    auto isWildcard = [](auto c) { return c == '*'; };

    str::trim_left(pattern, isWildcard);

    auto it = std::find_if(pattern.begin(), pattern.end(), isWildcard);

    return { pattern.begin(), it };
}

} // namespace adblock

#endif // ADBLOCK_INDEX_UTILITY_HPP
