#include "domain_match_filter_rule_map.hpp"

#include "utility.hpp"

#include "core/json.hpp"
#include "core/uri.hpp"
#include "pattern/domain_match_pattern.hpp"

#include <cassert>
#include <iterator>
#include <ostream>

#include <boost/range/algorithm.hpp>
#include <boost/property_tree/json_parser.hpp>

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

    return results;
}

json::object DomainMatchFilterRuleMap::
doStatistics() const
{
    std::ostringstream oss;

    boost::property_tree::write_json(oss, m_rules.statistics());

    return json::parse(oss.str()).get_object();
}

void DomainMatchFilterRuleMap::
doClear()
{
    m_rules.clear();
}

} // namespace adblock
