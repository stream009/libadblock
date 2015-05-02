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
    const auto &tokens = pattern->tokens();
    assert(!tokens.empty());
    m_rules.insert(tokens.front(), rule);
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

        m_rules.traverse(suffix,
            [&inserter] (const Rules::NodeType &node) {
                if (node.hasValue()) {
                    boost::copy(node.values(), inserter);
                }
                return false;
            }
        );
    }

    //TODO return lazy generator instead of copy
    return results;
}

void SubstringMatchFilterRuleSet::
doStatistics(std::ostream &os) const
{
    m_rules.statistics(os);
}

} // namespace adblock
