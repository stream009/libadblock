#include "prefix_match_filter_rule_map.hpp"

#include "statistics.hpp"
#include "utility.hpp"

#include "core/json.hpp"
#include "core/uri.hpp"
#include "pattern/basic_match_pattern.hpp"

#include <cassert>
#include <iterator>

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
            if (node.has_value()) {
                auto const& v = node.values();
                std::copy(v.begin(), v.end(), inserter);
            }
            return false;
        }
    );

    return results;
}

json::object PrefixMatchFilterRuleMap::
doStatistics() const
{
    return radixTreeStatistics(m_rules);
}

void PrefixMatchFilterRuleMap::
doClear()
{
    m_rules.clear();
}

} // namespace adblock
