#include "object_subrequest_option.hpp"

#include "context.hpp"

namespace adblock {

bool ObjectSubRequestOption::
doMatch2(const Uri&, const Context &context) const
{
    return context.fromPlugins();
}

} // namespace adblock
