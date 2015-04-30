#include "domained_element_hide_rule_set.hpp"

#include <iterator>

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/algorithm/cxx11/copy_if.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm.hpp>

namespace adblock {

using ElementHideRulePtr = DomainedElementHideRuleSet::ElementHideRulePtr;

static bool
excludedByDomain(const ElementHideRulePtr &rule, const StringRange &host)
{
    namespace ba = boost::algorithm;
    return ba::any_of(rule->excludeDomains(),
        [&host](const StringRange &domain) {
            return ba::ends_with(host, domain);
        }
    );
}

void DomainedElementHideRuleSet::
put(const ElementHideRulePtr &rule)
{
    assert(rule);
    assert(rule->isDomainRestricted());

    const auto &includes = rule->includeDomains();
    if (!includes.empty()) {
        for (const auto &domain: includes) {
            m_normal[domain].push_back(rule);
        }
    }
    else {
        // Rules which only have exclude domain mean
        // any domain except excluded domain.
        // So, register excluded domain for later query.
        const auto &excludes = rule->excludeDomains();
        assert(!excludes.empty());
        for (const auto &domain: excludes) {
            m_exception[domain].push_back(rule);
        }
    }
}

DomainedElementHideRuleSet::ElementHideRules DomainedElementHideRuleSet::
query(const Uri &uri) const
{
    assert(uri.is_valid());

    ElementHideRules results;
    const auto &inserter = std::back_inserter(results);

    const auto &host = uri.host_range();
    assert(!host.empty());
    const char *begin = &(*host.begin());
    const char* const end = begin + std::distance(host.begin(), host.end());

    const StringRange hostR { begin, end, };

    auto &&items = m_normal.match(hostR);
    if (items) {
        for (const auto &item: *items) {
            boost::algorithm::copy_if(item.second, inserter,
                [hostR](const ElementHideRulePtr &rule) {
                    return !excludedByDomain(rule, hostR);
                }
            );
        }
    }

    items = m_exception.match(hostR);
    if (!items) {
        for (const auto &item: m_exception) {
            boost::copy(item.second, inserter);
        }
    }
    else {
        // before match
        for (auto it = m_exception.begin(), end = items->begin();
             it != end;
             ++it)
        {
            boost::copy(it->second, inserter);
        }
        // after match
        for (auto it = items->end(), end = m_exception.end();
             it != end;
             ++it)
        {
            boost::copy(it->second, inserter);
        }
    }

    //TODO return lazy generator instead of copy
    return results;
}

} // namespace adblock
