#include "domain_match_filter_rule_map.hpp"

#include "utility.hpp"

#include "pattern/domain_match_pattern.hpp"

#include <cassert>
#include <iterator>
#include <ostream>

#include <boost/range/algorithm.hpp>

namespace adblock {

void DomainMatchFilterRuleMap::
doPut(FilterRule const& rule)
{
    auto* const pattern =
                dynamic_cast<DomainMatchPattern const*>(&rule.pattern());
    assert(pattern);

    auto const token = firstToken(pattern->domainPattern());
    m_rules.insert(token, &rule);
}

DomainMatchFilterRuleMap::FilterRules DomainMatchFilterRuleMap::
doQuery(Uri const& uri) const
{
    auto const& host = uri.host();
    if (host.empty()) return {};

    std::vector<FilterRule const*> results;
    auto inserter = std::back_inserter(results);

    char const* it = host.data();
    char const* const end = it + host.size();

    for (; it != end; ++it) {
        StringRange const suffix { it, end, };

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

boost::property_tree::ptree DomainMatchFilterRuleMap::
doStatistics() const
{
    return m_rules.statistics();
}

void DomainMatchFilterRuleMap::
doClear()
{
    m_rules.clear();
}

} // namespace adblock
