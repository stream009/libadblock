#include "domained_element_hide_rule_set.hpp"

#include <iterator>
#include <ostream>

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/algorithm/cxx11/copy_if.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
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
    assert(uri.is_valid());

    ElementHideRules results;
    auto &&inserter = std::back_inserter(results);

    const auto &host = uri.host_range();
    assert(!host.empty());
    const char *begin = &(*host.begin());
    const char* const end = begin + std::distance(host.begin(), host.end());

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
statistics(std::ostream &os) const
{
    m_normal.statistics(os);

    os << boost::format { "%20s: %6s\n" }
                    % "Exception Only Rules" % m_exception.size();
}

} // namespace adblock
