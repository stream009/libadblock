#ifndef REGEX_PATTERN_HPP
#define REGEX_PATTERN_HPP

#include "pattern.hpp"
#include "type.hpp"

#include <iosfwd>

#include <boost/regex.hpp>

namespace adblock {

class RegexPattern : public Pattern
{
public:
    RegexPattern(const StringRange&);

    bool match(const Uri &target, const bool caseSensitive = false) const override;

private:
    // @override Pattern
    void print(std::ostream&) const override;

private:
    StringRange m_pattern;
    mutable boost::optional<boost::regex> m_regEx;
    mutable bool m_regExCaseSensitivity = false;
    mutable bool m_disabled = false;
};

} // namespace adblock

#endif // REGEX_PATTERN_HPP
