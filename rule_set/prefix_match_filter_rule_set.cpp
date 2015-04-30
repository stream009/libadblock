#include "prefix_match_filter_rule_set.hpp"

#include "pattern/base_match_pattern.hpp"

#include <cassert>
#include <iterator>

#include <boost/range/algorithm.hpp>

namespace adblock {

void PrefixMatchFilterRuleSet::
doPut(const FilterRulePtr &rule)
{
    assert(rule);

    const auto *pattern =
                dynamic_cast<const BaseMatchPattern*>(&rule->pattern());
    assert(pattern);
    assert(pattern->isBeginMatch());
    //m_rules[pattern->firstToken()].push_back(rule);
    m_rules.insert(pattern->firstToken(), rule);
}

PrefixMatchFilterRuleSet::FilterRules PrefixMatchFilterRuleSet::
doQuery(const Uri &uri) const
{
    assert(uri.is_valid());

    std::vector<FilterRulePtr> results;
    const auto &inserter = std::back_inserter(results);

    const char *begin = &(*uri.begin());
    const char* const end = begin + std::distance(uri.begin(), uri.end());

    const StringRange uriR { begin, end, };
#if 0
    const auto &items = m_rules.match(uriR);
    if (items) {
        for (const auto &item: *items) {
            boost::copy(item.second, inserter);
        }
    }
#endif
    m_rules.traverse(uriR,
        [&inserter] (const Rules::NodeType &node) {
            boost::copy(node.values(), inserter);
            return false;
        }
    );
    //TODO return lazy generator instead of copy
    return results;
}

} // namespace adblock
