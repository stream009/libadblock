#ifndef BASIC_MATCH_PATTERN
#define BASIC_MATCH_PATTERN

#include "base_match_pattern.hpp"
#include "type.hpp"

namespace adblock {

class BasicMatchPattern : public BaseMatchPattern
{
    using Base = BaseMatchPattern;
public:
    BasicMatchPattern(const StringRange &range);
};

} // namespace adblock

#endif // BASIC_MATCH_PATTERN
