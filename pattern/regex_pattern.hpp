#ifndef REGEX_PATTERN_HPP
#define REGEX_PATTERN_HPP

#include "pattern.hpp"

#include <iosfwd>

#include <boost/regex.hpp>

class RegexPattern : public Pattern
{
public:
    RegexPattern(const Range &range);

    bool match(const Uri &target) const override;

private:
    // @override Pattern
    void print(std::ostream&) const override;

private:
    boost::regex m_regEx;
};

#endif // REGEX_PATTERN_HPP
