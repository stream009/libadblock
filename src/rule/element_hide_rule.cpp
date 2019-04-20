#include "element_hide_rule.hpp"

#include "utility.hpp"

#include "core/uri.hpp"

namespace adblock {

ElementHideRule::
ElementHideRule(StringRange const& selector,
                std::unique_ptr<Domains> domains)
    : m_cssSelector { selector }
    , m_domains { std::move(domains) }
{}

bool ElementHideRule::
match(Uri const& uri) const
{
    return matchDomain(uri.host(), m_domains.get());
}

void ElementHideRule::
print(std::ostream& os) const
{
    os << "CSS selector: " << m_cssSelector << "\n";
    if (m_domains) {
        os << "domains: ";
        for (const auto domain: *m_domains) {
            os << domain << ' ';
        }
        os << "\n";

    }
}

} // namespace adblock
