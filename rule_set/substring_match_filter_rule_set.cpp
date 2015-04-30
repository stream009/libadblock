#include "substring_match_filter_rule_set.hpp"

#include "pattern/base_match_pattern.hpp"

#include <cassert>
#include <iterator>

#include <boost/range/algorithm.hpp>

namespace adblock {

void SubstringMatchFilterRuleSet::
doPut(const FilterRulePtr &rule)
{
    assert(rule);

    const auto *pattern =
                dynamic_cast<const BaseMatchPattern*>(&rule->pattern());
    assert(pattern);
    m_map[pattern->firstToken()].push_back(rule);
}

SubstringMatchFilterRuleSet::FilterRules SubstringMatchFilterRuleSet::
doQuery(const Uri &uri) const
{
    assert(uri.is_valid());

    FilterRules results;
    const auto &inserter = std::back_inserter(results);

    const char *begin = &(*uri.begin());
    const char* const end = begin + std::distance(uri.begin(), uri.end());

    for (; begin != end; ++begin) {
        const StringRange suffix { begin, end, };

        const auto &items = m_map.match(suffix);
        if (!items) continue;

        std::cout << "SUFFIX: " << suffix << "\n";
        for (const auto &item: *items) {
            std::cout << "APPEND: " << "[" << item.first << "] ";
            for (const auto &rule: item.second) {
                std::cout << rule->pattern() << ", ";
            }
            std::cout << "\n";
            boost::copy(item.second, inserter);
        }
    }

    //TODO return lazy generator instead of copy
    return results;
}

} // namespace adblock
