#include "image_option.hpp"

#include "context.hpp"

namespace adblock {

bool ImageOption::
doMatch2(const Uri&, const Context &context) const
{
    return context.fromImageTag();
}

} // namespace adblock
