#include "object_option.hpp"

#include "context.hpp"

namespace adblock {

bool ObjectOption::
doMatch2(const Uri&, const Context &context) const
{
    return context.fromObjectTag();
}

} // namespace adblock
