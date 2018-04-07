#include "generic_hide_option.hpp"

#include "context.hpp"
#include "white_list_query_context.hpp"

namespace adblock {

bool GenericHideOption::
doMatch(Uri const&, Context const& cxt) const
{
    auto* const context = dynamic_cast<WhiteListQueryContext const*>(&cxt);
    if (context == nullptr) return false;

    return context->genericHideDisablerMode();
}

} // namespace adblock
