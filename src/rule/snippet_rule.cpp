#include "snippet_rule.hpp"

#include "utility.hpp"

#include "core/uri.hpp"

#include <ostream>

namespace adblock {

SnippetRule::
SnippetRule(StringRange snippet,
            std::unique_ptr<Domains> domains)
    : m_snippet { snippet }
    , m_domains { std::move(domains) }
{}

bool SnippetRule::
match(Uri const& uri) const
{
    return matchDomain(uri.host(), m_domains.get());
}

void SnippetRule::
print(std::ostream& os) const
{
    os << "snippet: " << m_snippet << "\n";
    if (m_domains) {
        os << "domains: ";
        for (auto const domain: *m_domains) {
            os << domain << ' ';
        }
        os << "\n";
    }
}

} // namespace adblock
