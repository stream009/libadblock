#include "domained_element_hide_rule_map.hpp"

#include "core/json.hpp"
#include "core/uri.hpp"

#include <iterator>
#include <sstream>
#include <string>

#include <boost/algorithm/cxx11/copy_if.hpp>
#include <boost/property_tree/json_parser.hpp>

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
    namespace ba = boost::algorithm;

    auto const& host = uri.host();
    if (host.empty()) return {};

    ElementHideRules results;
    auto inserter = std::back_inserter(results);

    char const* begin = host.data();
    char const* const end = begin + host.size();

    ReverseStringRange const reverseHostR { end, begin, };

    m_normal.traverse(reverseHostR,
        [&](auto& node, auto&) {
            if (node.hasValue()) {
                ba::copy_if(node.values(), inserter,
                    [&] (auto* rule) {
                        return rule->match(uri);
                    }
                );
            }
            return false;
        }
    );

    ba::copy_if(m_exception, inserter,
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
    std::ostringstream oss;

    boost::property_tree::write_json(oss, m_normal.statistics());

    auto result = json::parse(oss.str()).get_object();

    //TODO json::object::insert
    result["Exception only rules"] = static_cast<double>(m_exception.size());

    return result;
}

} // namespace adblock
