#include "substring_match_filter_rule_set.hpp"

#include "utility.hpp"

#include "pattern/base_match_pattern.hpp"

#include <cassert>
#include <iterator>
#include <ostream>

#include <boost/range/algorithm.hpp>

namespace adblock {

void SubstringMatchFilterRuleSet::
doPut(FilterRule const& rule)
{
    auto* const pattern =
                dynamic_cast<BaseMatchPattern const*>(&rule.pattern());
    assert(pattern);

    auto const token = firstToken(pattern->pattern());
    m_rules.insert(token, &rule);
}

SubstringMatchFilterRuleSet::FilterRules SubstringMatchFilterRuleSet::
doQuery(Uri const& uri) const
{
    FilterRules results;
    auto inserter = std::back_inserter(results);

    char const* begin = &(*uri.begin());
    char const* const end = begin + std::distance(uri.begin(), uri.end());

    for (; begin != end; ++begin) {
        StringRange const suffix { begin, end, };

        m_rules.traverse(suffix,
            [&](auto& node, auto&) {
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

boost::property_tree::ptree SubstringMatchFilterRuleSet::
doStatistics() const
{
    return m_rules.statistics();
}

void SubstringMatchFilterRuleSet::
doClear()
{
    m_rules.clear();
}

} // namespace adblock
