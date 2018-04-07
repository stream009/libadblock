#include "domained_element_hide_rule_set.hpp"

#include <iterator>
#include <ostream>
#include <string>
#include <utility>

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/algorithm/cxx11/copy_if.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/range/algorithm.hpp>

namespace adblock {

void DomainedElementHideRuleSet::
put(const ElementHideRule &rule)
{
    assert(rule.isDomainRestricted());

    const auto &includes = rule.includeDomains();
    if (!includes.empty()) {
        for (const auto &domain: includes) {
            const ReverseStringRange reversedDomain {
                                     domain.end(), domain.begin() };
            m_normal.insert(reversedDomain, &rule);
            //TODO duplication check
        }
    }
    else {
        // Rules which only have excluded domains mean
        // any domain except excluded domains.
        // So, register excluded domain for later query.
        const auto &excludes = rule.excludeDomains();
        assert(!excludes.empty());
        for (const auto &domain: excludes) {
            const ReverseStringRange reversedDomain {
                                     domain.end(), domain.begin() };
            m_exception.insert(&rule);
            //TODO duplication check
        }
    }
}

DomainedElementHideRuleSet::ElementHideRules DomainedElementHideRuleSet::
query(const Uri &uri) const
{
    const auto &host = uri.host();
    if (host.empty()) return {};

    ElementHideRules results;
    auto &&inserter = std::back_inserter(results);

    const char *begin = host.data();
    const char* const end = begin + host.size();

    namespace ba = boost::algorithm;
    const StringRange hostR { begin, end, };
    const ReverseStringRange reverseHostR { end, begin, };

    const auto &excludedDomain =
        [hostR](const ElementHideRule *rule) {
            return ba::any_of(rule->excludeDomains(),
                [&hostR](const StringRange &domain) {
                    return ba::ends_with(hostR, domain);
                }
            );
        };

    using Node = Rules::NodeType;
    m_normal.traverse(reverseHostR,
        [&inserter, &excludedDomain] (const Node &node, const Node::Key&) {
            if (node.hasValue()) {
                ba::copy_if(node.values(), inserter,
                    [&excludedDomain] (const ElementHideRule *rule) {
                        return !excludedDomain(rule);
                    }
                );
            }
            return false;
        }
    );

    ba::copy_if(m_exception, inserter,
        [&excludedDomain](const ElementHideRule *rule) {
            assert(rule->includeDomains().empty());
            return !excludedDomain(rule);
        }
    );

    //TODO return lazy generator instead of copy
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
