#include "regex_pattern.hpp"

#include <ostream>

namespace adblock {

RegexPattern::
RegexPattern(const StringRange &pattern)
    : m_pattern { pattern }
{}

bool RegexPattern::
match(const Uri &target, const bool caseSensitive/*= false*/) const
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
    catch (const std::exception &e) {
        std::cerr << "Problem has occur while processing an regex pattern.\n"
                  << "Pattern: " << m_pattern << "\n"
                  << "  Error: " << e.what() << "\n"
                  << "This pattern is disabled from now.\n";
        m_disabled = true;
        return false;
    }
}

void RegexPattern::
print(std::ostream &os) const
{
    os << m_pattern;
}

} // namespace adblock
