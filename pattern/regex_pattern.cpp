#include "regex_pattern.hpp"

RegexPattern::
RegexPattern(const Range &range)
    : m_regEx { range.begin(), range.end() }
{}

bool RegexPattern::
match(const Uri &target) const
{
    return boost::regex_match(target.begin(), target.end(), m_regEx);
}
