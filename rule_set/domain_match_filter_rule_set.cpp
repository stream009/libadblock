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
    assert(uri.is_valid());

    std::vector<const FilterRule*> results;
    const auto &inserter = std::back_inserter(results);

    const auto &host = uri.host_range();
    assert(!host.empty());
    const char *begin = &(*host.begin());
    const char* const end = begin + std::distance(host.begin(), host.end());

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

void DomainMatchFilterRuleSet::
doStatistics(std::ostream &os) const
{
    m_rules.statistics(os);
}

} // namespace adblock
