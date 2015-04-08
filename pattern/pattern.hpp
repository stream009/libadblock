#ifndef PATTERN_HPP
#define PATTERN_HPP

#include "type.hpp"

#include <ostream>
#include <typeinfo>

namespace adblock {

class Pattern
{
public:
    virtual ~Pattern() = default;

    virtual bool match(const Uri &target) const = 0;

private:
    virtual void print(std::ostream&) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const Pattern &pattern)
    {
        os << typeid(pattern).name() << " ";
        pattern.print(os);
        return os;
    }
};

} // namespace

#endif // PATTERN_HPP
