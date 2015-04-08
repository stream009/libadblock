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
    RegexPattern(const StringRange &range);

    bool match(const Uri &target) const override;

private:
    // @override Pattern
    void print(std::ostream&) const override;

private:
    boost::regex m_regEx;
};

} // namespace adblock

#endif // REGEX_PATTERN_HPP
