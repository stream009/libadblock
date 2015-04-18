#include "element_hide_rule.hpp"

#include <iterator>

namespace adblock {

ElementHideRule::
ElementHideRule(const StringRange &selector,
                const boost::optional<std::vector<StringRange>> &domains)
    : m_cssSelector { selector }
{
    if (!domains) return;

    for (const auto &domain: *domains) {
        if (domain.front() != '~') {
            addIncludeDomain(domain);
        }
        else {
            addExcludeDomain(
                StringRange {
                    std::next(domain.begin()), domain.end() }
            );
        }
    }
}

using DomainsRange = ElementHideRule::DomainsRange;

DomainsRange ElementHideRule::
includeDomains() const
{
    if (m_includeDomains) {
        return *m_includeDomains;
    }
    else {
        return DomainsRange {};
    }
}

DomainsRange ElementHideRule::
excludeDomains() const
{
    if (m_excludeDomains) {
        return *m_excludeDomains;
    }
    else {
        return DomainsRange {};
    }
}

void ElementHideRule::
addIncludeDomain(const StringRange &domain)
{
    if (m_includeDomains == boost::none) {
        m_includeDomains.emplace();
    }
    m_includeDomains->push_back(domain);
}

void ElementHideRule::
addExcludeDomain(const StringRange &domain)
{
    if (m_excludeDomains == boost::none) {
        m_excludeDomains.emplace();
    }
    m_excludeDomains->push_back(domain);
}

void ElementHideRule::
print(std::ostream &os) const
{
    os << "CSS selector: " << m_cssSelector << "\n";
    if (m_includeDomains) {
        os << "Include domains: ";
        for (const auto domain: *m_includeDomains) {
            os << domain << ' ';
        }
        os << "\n";
    }
    if (m_excludeDomains) {
        os << "Exclude domains: ";
        for (const auto domain: *m_excludeDomains) {
            os << domain << ' ';
        }
        os << "\n";
    }
}

} // namespace adblock
