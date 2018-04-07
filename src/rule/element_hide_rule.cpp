#include "element_hide_rule.hpp"

#include <iterator>

#include <boost/algorithm/cxx11/any_of.hpp>

namespace adblock {

ElementHideRule::
ElementHideRule(const StringRange &selector,
                const boost::optional<std::vector<StringRange>> &domains)
    : m_cssSelector { selector }
{
    assert(!selector.empty());

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

bool ElementHideRule::
match(const Uri &uri) const
{
    namespace ba = boost::algorithm;

    auto const& host = uri.host();
    const auto &domainMatch = [&host] (const StringRange &domain) {
        return ba::ends_with(host, domain);
    };

    if (m_excludeDomains &&
        ba::any_of(*m_excludeDomains, domainMatch))
    {
        return false;
    }

    if (m_includeDomains) {
        return ba::any_of(*m_includeDomains, domainMatch);
    }

    return true;
}

bool ElementHideRule::
isDomainRestricted() const
{
    return m_includeDomains || m_excludeDomains;
}

ElementHideRule::DomainsRange ElementHideRule::
includeDomains() const
{
    if (m_includeDomains) {
        return *m_includeDomains;
    }
    else {
        return DomainsRange {};
    }
}

ElementHideRule::DomainsRange ElementHideRule::
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
