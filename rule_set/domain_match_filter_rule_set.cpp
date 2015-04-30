#include "domain_match_filter_rule_set.hpp"

#include "pattern/domain_match_pattern.hpp"

#include <cassert>
#include <iterator>

#include <boost/range/algorithm.hpp>

namespace adblock {

void DomainMatchFilterRuleSet::
doPut(const FilterRulePtr &rule)
{
    assert(rule);

    const auto *pattern =
                dynamic_cast<const DomainMatchPattern*>(&rule->pattern());
    assert(pattern);

    m_map[pattern->firstToken()].push_back(rule);
}

DomainMatchFilterRuleSet::FilterRules DomainMatchFilterRuleSet::
doQuery(const Uri &uri) const
{
    assert(uri.is_valid());

    std::vector<FilterRulePtr> results;
    const auto &inserter = std::back_inserter(results);

    const auto &host = uri.host_range();
    assert(!host.empty());
    const char *begin = &(*host.begin());
    const char* const end = begin + std::distance(host.begin(), host.end());

    for (; begin != end; ++begin) {
        const StringRange suffix { begin, end, };

        const auto &items = m_map.match(suffix);
        if (!items) continue;

        for (const auto &item: *items) {
            boost::copy(item.second, inserter);
        }
    }

    //TODO return lazy generator instead of copy
    return results;
}

} // namespace adblock
