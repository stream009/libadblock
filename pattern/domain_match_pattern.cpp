#include "domain_match_pattern.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>

#include <boost/algorithm/string/predicate.hpp>

namespace adblock {

DomainMatchPattern::
DomainMatchPattern(const StringRange &domain,
                   const boost::optional<StringRange> &pattern)
    : Base { domain }, m_pattern { pattern }
{}

bool DomainMatchPattern::
doMatchUrl(const Uri &url) const
{
    static const Compare &compare {};

    if (!url.is_valid()) return false;

    const auto &host = url.host_range();
    if (!host) return false;

    // - Search begins from beginning of the host part.
    // - Head can progress only within the host part of the target.
    const auto searchBegin = host.begin();
    const auto searchEnd = url.end();
    const auto searchLimit = host.end();
    assert(searchBegin < searchEnd);
    assert(searchBegin < searchLimit);
    assert(searchLimit <= searchEnd);

    const auto &tokens = this->tokens();
    const auto &token = tokens.front();

    auto it = searchBegin;
    while (it != searchLimit) {
        namespace ba = boost::algorithm;

        const auto &search = boost::make_iterator_range(it, searchEnd);
        if (ba::starts_with(search, token, compare)) {
            break;
        }

        // Progress until the next dot (host separator).  TODO IPv6?
        it = std::find(it, searchLimit, '.');
        if (it == searchLimit) break;
        ++it;
    }

    if (it == searchLimit) return false;
    if (tokens.size() == 1) return true;

    // If first token satisfies domain match, rest are ok with basic match.
    assert(std::distance(it, searchEnd) >=
                             std::distance(token.begin(), token.end()));
    const auto &remaining = boost::make_iterator_range(
                            std::next(it, token.size()), searchEnd);

    assert(!tokens.empty());
    const auto &remainingTokens = boost::make_iterator_range(
                                  std::next(tokens.begin()), tokens.end());

    return Base::doMatch(remaining, remainingTokens);
}

} // namespace adblock
