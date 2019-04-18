#include "prefix_match_filter_rule_set.hpp"

#include "utility.hpp"

#include "pattern/basic_match_pattern.hpp"

#include <cassert>
#include <iterator>
#include <ostream>

#include <boost/range/algorithm.hpp>

namespace adblock {

void PrefixMatchFilterRuleSet::
doPut(const FilterRule &rule)
{
    auto* const pattern =
                dynamic_cast<BasicMatchPattern const*>(&rule.pattern());
    assert(pattern);
    assert(pattern->isBeginMatch());

    auto const token = firstToken(pattern->pattern());
    m_rules.insert(token, &rule);
}

PrefixMatchFilterRuleSet::FilterRules PrefixMatchFilterRuleSet::
doQuery(Uri const& uri) const
{
    std::vector<FilterRule const*> results;
    auto inserter = std::back_inserter(results);

    char const* begin = &(*uri.begin());
    char const* const end = begin + std::distance(uri.begin(), uri.end());

    StringRange const uriR { begin, end, };

    m_rules.traverse(uriR,
        [&](auto& node, auto&) {
            if (node.hasValue()) {
                boost::copy(node.values(), inserter);
            }
            return false;
        }
    );

    //TODO return lazy generator instead of copy
    return results;
}

boost::property_tree::ptree PrefixMatchFilterRuleSet::
doStatistics() const
{
    return m_rules.statistics();
}

void PrefixMatchFilterRuleSet::
doClear()
{
    m_rules.clear();
}

} // namespace adblock
