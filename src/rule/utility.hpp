#ifndef ADBLOCK_RULE_UTILITY_HPP
#define ADBLOCK_RULE_UTILITY_HPP

#include "core/string_range.hpp"

#include <vector>

namespace adblock {

bool matchDomain(StringRange host, std::vector<StringRange> const* domains);

} // namespace adblock

#endif // ADBLOCK_RULE_UTILITY_HPP
