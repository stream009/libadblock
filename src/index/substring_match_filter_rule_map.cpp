#include "substring_match_filter_rule_map.hpp"

#include "utility.hpp"

#include "core/json.hpp"
#include "core/uri.hpp"
#include "pattern/base_match_pattern.hpp"

#include <cassert>
#include <iterator>
#include <sstream>

#include <boost/property_tree/json_parser.hpp>
#include <boost/range/algorithm.hpp>

namespace adblock {

void SubstringMatchFilterRuleMap::
doPut(FilterRule const& rule)
{
    auto* const pattern =
                dynamic_cast<BaseMatchPattern const*>(&rule.pattern());
    assert(pattern);

    auto const token = firstToken(pattern->pattern());
    m_rules.insert(token, &rule);
}

SubstringMatchFilterRuleMap::FilterRules SubstringMatchFilterRuleMap::
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

    return results;
}

json::object SubstringMatchFilterRuleMap::
doStatistics() const
{
    std::ostringstream oss;

    boost::property_tree::write_json(oss, m_rules.statistics());

    return json::parse(oss.str()).get_object();
}

void SubstringMatchFilterRuleMap::
doClear()
{
    m_rules.clear();
}

} // namespace adblock
