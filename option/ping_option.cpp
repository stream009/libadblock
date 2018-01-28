#include "ping_option.hpp"

#include "context.hpp"

namespace adblock {

bool PingOption::
doMatch2(Uri const&, Context const& context) const
{
    return context.isPing();
}

} // namespace adblock
