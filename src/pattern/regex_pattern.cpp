#include "regex_pattern.hpp"

#include "core/uri.hpp"

#include <iostream>

namespace adblock {

RegexPattern::
RegexPattern(StringRange const pattern)
    : m_pattern { pattern }
{}

bool RegexPattern::
match(Uri const& target, bool const caseSensitive/*= false*/) const
{
    if (m_disabled) return false;

    try {
        if (!m_regEx || caseSensitive != m_regExCaseSensitivity) {

            auto syntax = std::regex::ECMAScript;
            if (!caseSensitive) {
                syntax |= std::regex::icase;
            }

            m_regEx = std::make_unique<std::regex>(
                m_pattern.begin(), m_pattern.end(),
                syntax
            );

            m_regExCaseSensitivity = caseSensitive;
        }
        return std::regex_match(target.begin(), target.end(), *m_regEx);
    }
    catch (std::exception const& e) {
        std::cerr << "regex error: " << m_pattern << ":" << e.what() << "\n";
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
