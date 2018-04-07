#include "domain_match_filter_rule_set.hpp"

#include "pattern/domain_match_pattern.hpp"

#include <cassert>
#include <iterator>
#include <ostream>

#include <boost/range/algorithm.hpp>

namespace adblock {

void DomainMatchFilterRuleSet::
doPut(const FilterRule &rule)
{
    const auto *pattern =
                dynamic_cast<const DomainMatchPattern*>(&rule.pattern());
    assert(pattern);

    const auto &tokens = pattern->tokens();
    assert(!tokens.empty());
    m_rules.insert(tokens.front(), &rule);
}

DomainMatchFilterRuleSet::FilterRules DomainMatchFilterRuleSet::
doQuery(const Uri &uri) const
{
    const auto &host = uri.host();
    if (host.empty()) return {};

    std::vector<const FilterRule*> results;
    const auto &inserter = std::back_inserter(results);

    const char *begin = host.data();
    const char* const end = begin + host.size();

    for (; begin != end; ++begin) {
        const StringRange suffix { begin, end, };

        using Node = Rules::NodeType;
        m_rules.traverse(suffix,
            [&inserter] (const Node &node, const Node::Key&) {
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

boost::property_tree::ptree DomainMatchFilterRuleSet::
doStatistics() const
{
    return m_rules.statistics();
}

void DomainMatchFilterRuleSet::
doClear()
{
    m_rules.clear();
}

} // namespace adblock
