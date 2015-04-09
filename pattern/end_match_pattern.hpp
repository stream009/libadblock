#ifndef END_MATCH_PATTERN_HPP
#define END_MATCH_PATTERN_HPP

#include "base_match_pattern.hpp"
#include "type.hpp"

namespace adblock {

class EndMatchPattern : public BaseMatchPattern
{
    using Base = BaseMatchPattern;
public:
    using Base::TokenRange;
    using Base::UriRange;

public:
    EndMatchPattern(const StringRange &range);

private:
    // @override BaseMatchPattern
    bool doMatch(const UriRange&, const TokenRange&) const override;
};

} // namespace adblock

#endif // END_MATCH_PATTERN_HPP
