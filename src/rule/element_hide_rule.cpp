#include "element_hide_rule.hpp"

#include <iterator>

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/algorithm/string.hpp>

namespace adblock {

ElementHideRule::
ElementHideRule(StringRange const& selector,
                Domains const& domains)
    : m_cssSelector { selector }
{
    assert(!selector.empty());

    for (auto const& domain: domains) {
        if (domain.front() != '~') {
            m_includeDomains.push_back(domain);
        }
        else {
            m_excludeDomains.emplace_back(
                std::next(domain.begin()), domain.end()
            );
        }
    }
}

bool ElementHideRule::
match(Uri const& uri) const
{
    namespace ba = boost::algorithm;

    auto const& host = uri.host();
    auto const& domainMatch = [&host] (StringRange const& domain) {
        return ba::ends_with(host, domain);
    };

    if (ba::any_of(m_excludeDomains, domainMatch)) {
        return false;
    }

    if (m_includeDomains.empty()) {
        return true;
    }
    else {
        return ba::any_of(m_includeDomains, domainMatch);
    }
}

bool ElementHideRule::
isDomainRestricted() const
{
    return !m_includeDomains.empty()
        || !m_excludeDomains.empty();
}

void ElementHideRule::
print(std::ostream &os) const
{
    os << "CSS selector: " << m_cssSelector << "\n";
    if (!m_includeDomains.empty()) {
        os << "Include domains: ";
        for (const auto domain: m_includeDomains) {
            os << domain << ' ';
        }
        os << "\n";
    }
    if (!m_excludeDomains.empty()) {
        os << "Exclude domains: ";
        for (const auto domain: m_excludeDomains) {
            os << domain << ' ';
        }
        os << "\n";
    }
}

} // namespace adblock
