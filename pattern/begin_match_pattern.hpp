#ifndef BEGIN_MATCH_PATTERN_HPP
#define BEGIN_MATCH_PATTERN_HPP

#include "type.hpp"
#include "basic_match_pattern.hpp"

namespace adblock {

class BeginMatchPattern : public BasicMatchPattern
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

#endif // BEGIN_MATCH_PATTERN_HPP
