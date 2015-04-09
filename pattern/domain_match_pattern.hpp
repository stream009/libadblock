#ifndef DOMAIN_MATCH_PATTERN_HPP
#define DOMAIN_MATCH_PATTERN_HPP

#include "base_match_pattern.hpp"
#include "type.hpp"

#include <boost/optional.hpp>

namespace adblock {

class DomainMatchPattern : public BaseMatchPattern
{
    using Base = BaseMatchPattern;
public:
    DomainMatchPattern(const StringRange &domain,
                       const boost::optional<StringRange> &pattern);

private:
    // @override BaseMatchPattern
    bool doMatchUrl(const Uri&) const override;

private:
    boost::optional<StringRange> m_pattern;
};

} // namespace adblock

#endif // DOMAIN_MATCH_PATTERN_HPP
