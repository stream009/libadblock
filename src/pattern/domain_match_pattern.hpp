#ifndef DOMAIN_MATCH_PATTERN_HPP
#define DOMAIN_MATCH_PATTERN_HPP

#include "base_match_pattern.hpp"
#include "core/type.hpp"

namespace adblock {

class DomainMatchPattern : public BaseMatchPattern
{
    using Base = BaseMatchPattern;
    using Base::Tokens;
public:
    DomainMatchPattern(StringRange const& pattern,
                       bool endMatch = false);

    // query
    bool isEndMatch() const { return m_endAnchor; }
    StringRange domainPattern() const;

private:
    // @override BaseMatchPattern
    bool doMatchUrl(Uri const&, bool caseSensitive) const override;

private:
    bool m_endAnchor = false;
};

} // namespace adblock

#endif // DOMAIN_MATCH_PATTERN_HPP
