#include "element_hide_rule.hpp"

namespace adblock {

ElementHideRule::
ElementHideRule(const StringRange &selector,
                const boost::optional<std::vector<StringRange>> &domains)
    : m_cssSelector { selector }
{
    if (domains) {
        for (const auto &domain: *domains) {
            if (domain.front() != '~') {
                includeDomains().push_back(domain);
            }
            else {
                excludeDomains().push_back(domain);
            }
        }
    }
}

std::vector<StringRange> &ElementHideRule::
includeDomains()
{
    if (m_includeDomains == boost::none) {
        m_includeDomains.emplace();
    }
    return *m_includeDomains;
}

std::vector<StringRange> &ElementHideRule::
excludeDomains()
{
    if (m_excludeDomains == boost::none) {
        m_excludeDomains.emplace();
    }
    return *m_excludeDomains;
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
