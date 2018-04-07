#include "suffix_match_filter_rule_set.hpp"

#include "pattern/base_match_pattern.hpp"

#include <cassert>
#include <iterator>
#include <ostream>

#include <boost/range/algorithm.hpp>

namespace adblock {

void SuffixMatchFilterRuleSet::
doPut(const FilterRule &rule)
{
    const auto *pattern =
                dynamic_cast<const BaseMatchPattern*>(&rule.pattern());
    assert(pattern);
    assert(pattern->isEndMatch());
    const auto &tokens = pattern->tokens();
    assert(!tokens.empty());
    const auto &token = tokens.back();
    const ReverseStringRange reverseToken { token.end(), token.begin() };
    m_rules.insert(reverseToken, &rule);
}

SuffixMatchFilterRuleSet::FilterRules SuffixMatchFilterRuleSet::
doQuery(const Uri &uri) const
{
    std::vector<const FilterRule*> results;
    const auto &inserter = std::back_inserter(results);

    const char *begin = &(*uri.begin());
    const char* const end = begin + std::distance(uri.begin(), uri.end());

    const ReverseStringRange uriR { end, begin };

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

boost::property_tree::ptree SuffixMatchFilterRuleSet::
doStatistics() const
{
    return m_rules.statistics();
}

void SuffixMatchFilterRuleSet::
doClear()
{
    m_rules.clear();
}

} // namespace adblock
