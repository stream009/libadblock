#ifndef BEGIN_MATCH_PATTERN_HPP
#define BEGIN_MATCH_PATTERN_HPP

#include "basic_match_pattern.hpp"

class BeginMatchPattern : public BasicMatchPattern
{
    using Base = BasicMatchPattern;
public:
    using Base::Base;

private:
    // @override BasicMatchPattern
    bool doMatch(const Range&, const TokenRange&) const override;
};

#endif // BEGIN_MATCH_PATTERN_HPP
