#include "prefix_match_filter_rule_set.hpp"

#include "pattern/base_match_pattern.hpp"

#include <cassert>
#include <iterator>
#include <ostream>

#include <boost/range/algorithm.hpp>

namespace adblock {

void PrefixMatchFilterRuleSet::
doPut(const FilterRule &rule)
{
    const auto *pattern =
                dynamic_cast<const BaseMatchPattern*>(&rule.pattern());
    assert(pattern);
    assert(pattern->isBeginMatch());
    const auto &tokens = pattern->tokens();
    assert(!tokens.empty());
    m_rules.insert(tokens.front(), &rule);
}

PrefixMatchFilterRuleSet::FilterRules PrefixMatchFilterRuleSet::
doQuery(const Uri &uri) const
{
    assert(uri.is_valid());

    std::vector<const FilterRule*> results;
    const auto &inserter = std::back_inserter(results);

    const char *begin = &(*uri.begin());
    const char* const end = begin + std::distance(uri.begin(), uri.end());

    const StringRange uriR { begin, end, };

    using Node = Rules::NodeType;
    m_rules.traverse(uriR,
        [&inserter] (const Node &node, const Node::Key&) {
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
