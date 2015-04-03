#ifndef END_MATCH_PATTERN_HPP
#define END_MATCH_PATTERN_HPP

#include "basic_match_pattern.hpp"

class EndMatchPattern : public BasicMatchPattern
{
    using Base = BasicMatchPattern;
public:
    using Base::Base;

private:
    // @override BasicMatchPattern
    bool doMatch(const Range&, const TokenRange&) const override;
};

#endif // END_MATCH_PATTERN_HPP
