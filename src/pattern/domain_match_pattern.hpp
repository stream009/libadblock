#ifndef DOMAIN_MATCH_PATTERN_HPP
#define DOMAIN_MATCH_PATTERN_HPP

#include "base_match_pattern.hpp"
#include "type.hpp"

namespace adblock {

class DomainMatchPattern : public BaseMatchPattern
{
    using Base = BaseMatchPattern;
    using Base::Tokens;
public:
    DomainMatchPattern(StringRange const& pattern,
                       bool endMatch = false);

    bool isEndMatch() const { return m_endAnchor; }

private:
    // @override BaseMatchPattern
    bool doMatchUrl(Uri const&) const override;
    Tokens doTokens() const override;

private:
    bool m_endAnchor = false;
};

} // namespace adblock

#endif // DOMAIN_MATCH_PATTERN_HPP
