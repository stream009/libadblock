#ifndef OPTION_HPP
#define OPTION_HPP

#include "context.hpp"
#include "type.hpp"

#include <ostream>
#include <typeinfo>

namespace adblock {

class Option
{
public:
    virtual ~Option() = default;

    bool match(const Uri &uri, const Context &context) const
    {
        return doMatch(uri, context);
    }

protected:
    virtual bool doMatch(const Uri&, const Context&) const
    {
        return false;
    }
};

inline std::ostream &
operator<<(std::ostream &os, const Option &option)
{
    return os << typeid(option).name() << " ";
}

} // namespace adblock

#endif // OPTION_HPP
