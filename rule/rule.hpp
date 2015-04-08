#ifndef RULE_HPP
#define RULE_HPP

#include "type.hpp"

#include <ostream>
#include <typeinfo>

#include <boost/noncopyable.hpp>

namespace adblock {

class Rule : boost::noncopyable
{
public:
    virtual ~Rule() = default;

private:
    virtual void print(std::ostream&) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const Rule &rule)
    {
        os << typeid(rule).name() << "\n";
        rule.print(os);
        return os;
    }
};

} // namespace adblock

#endif // RULE_HPP
