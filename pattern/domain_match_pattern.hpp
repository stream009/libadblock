#ifndef DOMAIN_MATCH_PATTERN_HPP
#define DOMAIN_MATCH_PATTERN_HPP

#include "basic_match_pattern.hpp"

namespace adblock {

class DomainMatchPattern : public BasicMatchPattern
{
    using Base = BasicMatchPattern;
public:
    using Base::Base;

private:
    // @override BasicMatchPattern
    bool doMatchUrl(const Uri&) const override;
};

} // namespace adblock

#endif // DOMAIN_MATCH_PATTERN_HPP
