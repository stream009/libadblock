#include "substring_match_filter_rule_map.hpp"

#include "statistics.hpp"
#include "utility.hpp"

#include "core/json.hpp"
#include "core/uri.hpp"
#include "pattern/base_match_pattern.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>

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
                if (node.has_value()) {
                    auto const& v = node.values();
                    std::copy(v.begin(), v.end(), inserter);
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
    return radixTreeStatistics(m_rules);
}

void SubstringMatchFilterRuleMap::
doClear()
{
    m_rules.clear();
}

} // namespace adblock
