#ifndef REGEX_PATTERN_HPP
#define REGEX_PATTERN_HPP

#include "pattern.hpp"

#include "core/string_range.hpp"

#include <iosfwd>
#include <memory>

#include <boost/regex_fwd.hpp>

namespace adblock {

class Uri;

class RegexPattern : public Pattern
{
public:
    RegexPattern(StringRange const);

    ~RegexPattern() noexcept override;

    bool match(Uri const& target, bool caseSensitive = false) const override;

private:
    // @override Pattern
    void print(std::ostream&) const override;

private:
    StringRange m_pattern;
    mutable std::unique_ptr<boost::regex> m_regEx;
    mutable bool m_regExCaseSensitivity = false;
    mutable bool m_disabled = false;
};

} // namespace adblock

#endif // REGEX_PATTERN_HPP
