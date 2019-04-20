#include "regex_pattern.hpp"

#include "core/uri.hpp"

#include <iostream>

namespace adblock {

RegexPattern::
RegexPattern(StringRange const& pattern)
    : m_pattern { pattern }
{}

bool RegexPattern::
match(Uri const& target, bool const caseSensitive/*= false*/) const
{
    if (m_disabled) return false;

    try {
        if (!m_regEx || caseSensitive != m_regExCaseSensitivity) {
            m_regEx.emplace(
                m_pattern.begin(), m_pattern.end(),
                caseSensitive ? boost::regex::normal : boost::regex::icase
            );
            m_regExCaseSensitivity = caseSensitive;
        }
        return boost::regex_match(target.begin(), target.end(), *m_regEx);
    }
    catch (std::exception const& e) {
        std::cerr << "Problem has occur while processing an regex pattern.\n"
                  << "Pattern: " << m_pattern << "\n"
                  << "  Error: " << e.what() << "\n"
                  << "This pattern is disabled from now.\n";
        m_disabled = true;
        return false;
    }
}

void RegexPattern::
print(std::ostream& os) const
{
    os << m_pattern;
}

} // namespace adblock
