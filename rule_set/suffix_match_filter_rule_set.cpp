#include "suffix_match_filter_rule_set.hpp"

#include "pattern/base_match_pattern.hpp"

#include <cassert>
#include <iterator>

#include <boost/range/algorithm.hpp>

namespace adblock {

void SuffixMatchFilterRuleSet::
doPut(const FilterRulePtr &rule)
{
    assert(rule);

    const auto *pattern =
                dynamic_cast<const BaseMatchPattern*>(&rule->pattern());
    assert(pattern);
    assert(pattern->isEndMatch());
    m_map[pattern->firstToken()].push_back(rule);
}

SuffixMatchFilterRuleSet::FilterRules SuffixMatchFilterRuleSet::
doQuery(const Uri &uri) const
{
    assert(uri.is_valid());

    std::vector<FilterRulePtr> results;
    const auto &inserter = std::back_inserter(results);

    const char *begin = &(*uri.begin());
    const char* const end = begin + std::distance(uri.begin(), uri.end());

    const StringRange uriR { begin, end, };

    const auto &items = m_map.match(uriR);
    if (items) {
        for (const auto &item: *items) {
            boost::copy(item.second, inserter);
        }
    }

    //TODO return lazy generator instead of copy
    return results;
}

} // namespace adblock
