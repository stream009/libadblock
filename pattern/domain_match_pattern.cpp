#include "domain_match_pattern.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>

#include <boost/algorithm/string.hpp>

namespace adblock {

DomainMatchPattern::
DomainMatchPattern(const StringRange &domain,
                   const boost::optional<StringRange> &pattern,
                   const bool endMatch)
    : Base { StringRange { domain.begin(),
                           pattern ? pattern->end() : domain.end() },
             false, endMatch }
{
    namespace ba = boost::algorithm;

    const auto &trimmedDomain = ba::trim_copy_if(domain, ba::is_any_of("*"));
    ba::split(m_domainTokens, trimmedDomain,
              ba::is_any_of("*"), ba::token_compress_on);

    if (pattern) {
        const auto &trimmedPattern =
            ba::trim_copy_if(*pattern, ba::is_any_of("*"));
        ba::split(m_patternTokens, trimmedPattern,
                ba::is_any_of("*"), ba::token_compress_on);
    }
}

bool DomainMatchPattern::
doMatchUrl(const Uri &url) const
{
    const auto &host = url.host_range();
    if (!this->doMatch(host, m_domainTokens)) return false;

    return this->doMatch(
            UriRange { host.end(), url.end() }, m_patternTokens);
}

} // namespace adblock
