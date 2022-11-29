#include "suffix_match_filter_rule_map.hpp"

#include "namespace.hpp"
#include "statistics.hpp"

#include "core/json.hpp"
#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "pattern/basic_match_pattern.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>

#include <stream9/strings/trim.hpp>

namespace adblock {

static StringRange
lastToken(StringRange pattern)
{
    auto isWildcard = [](auto c) { return c == '*'; };

    str::trim_right(pattern, isWildcard);

    auto it = pattern.end();
    auto begin = pattern.begin();

    while (it != begin) {
        --it;
        if (*it == '*') break;
    }

    if (it == begin) {
        return { begin, pattern.end() };
    }
    else {
        return { it + 1, pattern.end() };
    }
}

void SuffixMatchFilterRuleMap::
doPut(FilterRule const& rule)
{
    auto* const pattern =
                dynamic_cast<BasicMatchPattern const*>(&rule.pattern());
    assert(pattern);
    assert(pattern->isEndMatch());

    auto const token = lastToken(pattern->pattern());
    ReverseStringRange const reverseToken { token.end(), token.begin() };
    m_rules.insert(reverseToken, &rule);
}

SuffixMatchFilterRuleMap::FilterRules SuffixMatchFilterRuleMap::
doQuery(Uri const& uri) const
{
    std::vector<FilterRule const*> results;
    auto inserter = std::back_inserter(results);

    char const* begin = &(*uri.begin());
    char const* const end = begin + std::distance(uri.begin(), uri.end());

    ReverseStringRange const uriR { end, begin };

    m_rules.traverse(uriR,
        [&](auto &node, auto&) {
            if (node.has_value()) {
                auto const& v = node.values();
                std::copy(v.begin(), v.end(), inserter);
            }
            return false;
        }
    );

    return results;
}

json::object SuffixMatchFilterRuleMap::
doStatistics() const
{
    return radixTreeStatistics(m_rules);
}

void SuffixMatchFilterRuleMap::
doClear()
{
    m_rules.clear();
}

} // namespace adblock
