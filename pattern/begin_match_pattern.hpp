#ifndef BEGIN_MATCH_PATTERN_HPP
#define BEGIN_MATCH_PATTERN_HPP

#include "type.hpp"
#include "base_match_pattern.hpp"

namespace adblock {

class BeginMatchPattern : public BaseMatchPattern
{
    using Base = BaseMatchPattern;
public:
    using Base::TokenRange;
    using Base::UriRange;

public:
    BeginMatchPattern(const StringRange &range);

private:
    // @override BaseMatchPattern
    bool doMatch(const UriRange&, const TokenRange&) const override;
};

} // namespace adblock

#endif // BEGIN_MATCH_PATTERN_HPP
