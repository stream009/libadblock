#ifndef END_MATCH_PATTERN_HPP
#define END_MATCH_PATTERN_HPP

#include "basic_match_pattern.hpp"
#include "type.hpp"

namespace adblock {

class EndMatchPattern : public BasicMatchPattern
{
    using Base = BasicMatchPattern;
public:
    using Base::TokenRange;
    using Base::UriRange;

public:
    using Base::Base;

private:
    // @override BasicMatchPattern
    bool doMatch(const UriRange&, const TokenRange&) const override;
};

} // namespace adblock

#endif // END_MATCH_PATTERN_HPP
