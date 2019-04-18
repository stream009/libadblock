#include "domained_element_hide_rule_set.hpp"

#include <ostream>
#include <string>

#include <boost/algorithm/cxx11/copy_if.hpp>
#include <boost/property_tree/ptree.hpp>

namespace adblock {

void DomainedElementHideRuleSet::
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
            //TODO duplication check
        }
        else {
            hasExclude = true;
        }
    }

    if (hasExclude && !hasInclude) {
        for (auto& dom: *domains) {
            ReverseStringRange const revDom { dom.end(), dom.begin() };

            m_exception.insert(&rule);
        }
    }
}

DomainedElementHideRuleSet::ElementHideRules DomainedElementHideRuleSet::
query(Uri const& uri) const
{
    auto const& host = uri.host();
    if (host.empty()) return {};

    ElementHideRules results;
    auto inserter = std::back_inserter(results);

    char const* begin = host.data();
    char const* const end = begin + host.size();

    namespace ba = boost::algorithm;
    StringRange const hostR { begin, end, };
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

    //TODO more efficiently
    ba::copy_if(m_exception, inserter,
        [&](auto* rule) {
            return rule->match(uri);
        }
    );

    return results;
}

void DomainedElementHideRuleSet::
clear()
{
    m_normal.clear();
    m_exception.clear();
}

boost::property_tree::ptree DomainedElementHideRuleSet::
statistics() const
{
    auto &&result = m_normal.statistics();

    auto assocIt = result.find("Number of values");
    assert(assocIt != result.not_found());
    auto &&it = result.to_iterator(assocIt);
    ++it; // insert AFTER "Number of values"

    namespace bfp = boost::property_tree;
    bfp::ptree value { std::to_string(m_exception.size()) };
    bfp::ptree::value_type item { "Exception only rules", value };
    result.insert(it, item);

    return result;
}

} // namespace adblock
