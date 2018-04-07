#include "font_option.hpp"

#include "context.hpp"

namespace adblock {

bool FontOption::
doMatch2(const Uri&, const Context &context) const
{
    return context.isFont();
}

} // namespace adblock
