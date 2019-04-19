#ifndef ADBLOCK_INDEX_UTILITY_HPP
#define ADBLOCK_INDEX_UTILITY_HPP

#include "type.hpp"

#include <algorithm>

#include <boost/algorithm/string.hpp>

namespace adblock {

inline StringRange
firstToken(StringRange pattern)
{
    namespace ba = boost::algorithm;

    auto isWildcard = [](auto c) { return c == '*'; };

    pattern = ba::trim_left_copy_if(pattern, isWildcard);

    auto const it = std::find_if(pattern.begin(), pattern.end(), isWildcard);

    return { pattern.begin(), it };
}

} // namespace adblock

#endif // ADBLOCK_INDEX_UTILITY_HPP
