#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <ostream>

namespace adblock {

class Uri;

class Pattern
{
public:
    virtual ~Pattern() = default;

    virtual bool match(Uri const& target,
                       bool caseSensitive = false) const = 0;
private:
    virtual void print(std::ostream&) const = 0;

    friend std::ostream &operator<<(std::ostream &os, Pattern const& pattern)
    {
        pattern.print(os);
        return os;
    }
};

} // namespace

#endif // PATTERN_HPP
