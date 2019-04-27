#include "prefix_match_filter_rule_map.hpp"

#include "utility.hpp"

#include "core/json.hpp"
#include "core/uri.hpp"
#include "pattern/basic_match_pattern.hpp"

#include <cassert>
#include <iterator>
#include <sstream>

#include <boost/range/algorithm.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace adblock {

void PrefixMatchFilterRuleMap::
doPut(FilterRule const& rule)
{
    auto* const pattern =
                dynamic_cast<BasicMatchPattern const*>(&rule.pattern());
    assert(pattern);
    assert(pattern->isBeginMatch());

    auto const token = firstToken(pattern->pattern());
    m_rules.insert(token, &rule);
}

PrefixMatchFilterRuleMap::FilterRules PrefixMatchFilterRuleMap::
doQuery(Uri const& uri) const
{
    FilterRules results;
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

    return results;
}

json::object PrefixMatchFilterRuleMap::
doStatistics() const
{
    std::ostringstream oss;

    boost::property_tree::write_json(oss, m_rules.statistics());

    return json::parse(oss.str()).get_object();
}

void PrefixMatchFilterRuleMap::
doClear()
{
    m_rules.clear();
}

} // namespace adblock
