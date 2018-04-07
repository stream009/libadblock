#include "popup_option.hpp"

#include "context.hpp"

namespace adblock {

bool PopUpOption::
doMatch(const Uri&, const Context &context) const
{
    return context.isPopUp();
}

} // namespace adblock
