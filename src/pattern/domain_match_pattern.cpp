#include "domain_match_pattern.hpp"

#include <algorithm>
#include <iterator>

#include <boost/algorithm/string.hpp>

namespace adblock {

DomainMatchPattern::
DomainMatchPattern(StringRange const& domain,
                   StringRange const& pattern,
                   bool const endMatch/*= false*/)
    : Base { StringRange { domain.begin(), pattern.end() }, false, endMatch }
    , m_pathPattern { pattern, true, endMatch }
{
    namespace ba = boost::algorithm;
    assert(!domain.empty());

    auto const& trimmedDomain = ba::trim_copy_if(domain, ba::is_any_of("*"));
    ba::split(m_domainTokens, trimmedDomain,
              ba::is_any_of("*"), ba::token_compress_on);
}

bool DomainMatchPattern::
doMatchUrl(Uri const& url) const
{
    auto const& host = url.host();
    assert(!host.empty());
    StringRange const hostR  { host.begin(), host.end() };

    if (!this->doMatch(hostR, m_domainTokens)) return false;

    StringRange range { hostR.end(), &*url.end() };
    return m_pathPattern.match(range);
}

} // namespace adblock
