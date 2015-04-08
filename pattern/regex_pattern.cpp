#include "regex_pattern.hpp"

#include <ostream>

namespace adblock {

RegexPattern::
RegexPattern(const StringRange &range)
    : m_regEx { range.begin(), range.end() }
{}

bool RegexPattern::
match(const Uri &target) const
{
    return boost::regex_match(target.begin(), target.end(), m_regEx);
}

void RegexPattern::
print(std::ostream &os) const
{
    os << m_regEx.str();
}

} // namespace adblock
