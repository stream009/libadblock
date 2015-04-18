#ifndef DOMAIN_MATCH_PATTERN_HPP
#define DOMAIN_MATCH_PATTERN_HPP

#include "base_match_pattern.hpp"
#include "type.hpp"

#include <boost/optional.hpp>

namespace adblock {

class DomainMatchPattern : public BaseMatchPattern
{
    using Base = BaseMatchPattern;
    using Base::Tokens;
public:
    DomainMatchPattern(const StringRange &domain,
                       const boost::optional<StringRange> &pattern,
                       const bool endMatch);

private:
    // @override BaseMatchPattern
    bool doMatchUrl(const Uri&) const override;
    const Tokens &tokens() const override { return m_domainTokens; }

private:
    Tokens m_domainTokens;
    Tokens m_patternTokens;
};

} // namespace adblock

#endif // DOMAIN_MATCH_PATTERN_HPP
