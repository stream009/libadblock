#include "domained_element_hide_rule_map.hpp"

#include "statistics.hpp"

#include "core/json.hpp"
#include "core/uri.hpp"

#include <algorithm>
#include <iterator>
#include <string>

namespace adblock {

void DomainedElementHideRuleMap::
put(ElementHideRule const& rule)
{
    assert(rule.isDomainRestricted());

    auto* const domains = rule.domains();
    assert(domains);

    bool hasInclude = false, hasExclude = false;

    for (auto& dom: *domains) {
        // [[assert(!dom.empty())]]

        if (dom[0] != '~') {
            hasInclude = true;
            ReverseStringRange const revDom { dom.end(), dom.begin() };

            m_normal.insert(revDom, &rule);
        }
        else {
            hasExclude = true;
        }
    }

    if (hasExclude && !hasInclude) {
        m_exception.push_back(&rule);
    }
}

DomainedElementHideRuleMap::ElementHideRules DomainedElementHideRuleMap::
query(Uri const& uri) const
{
    auto const& host = uri.host();
    if (host.empty()) return {};

    ElementHideRules results;
    auto inserter = std::back_inserter(results);

    char const* begin = host.data();
    char const* const end = begin + host.size();

    ReverseStringRange const reverseHostR { end, begin, };

    m_normal.traverse(reverseHostR,
        [&](auto& node, auto&) {
            if (node.has_value()) {
                auto const& v = node.values();

                std::copy_if(
                    v.begin(), v.end(), inserter,
                    [&] (auto* rule) {
                        return rule->match(uri);
                    }
                );
            }
            return false;
        }
    );

    std::copy_if(
        m_exception.begin(), m_exception.end(),
        inserter,
        [&](auto* rule) {
            return rule->match(uri);
        }
    );

    return results;
}

void DomainedElementHideRuleMap::
clear()
{
    m_normal.clear();
    m_exception.clear();
}

json::object DomainedElementHideRuleMap::
statistics() const
{
    auto result = radixTreeStatistics(m_normal);

    //TODO json::object::insert
    result["Exception only rules"] = static_cast<double>(m_exception.size());

    return result;
}

} // namespace adblock
