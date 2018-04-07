#include "generic_block_option.hpp"

#include "context.hpp"
#include "white_list_query_context.hpp"

namespace adblock {

bool GenericBlockOption::
doMatch(Uri const&, Context const& cxt) const
{
    auto* const context = dynamic_cast<WhiteListQueryContext const*>(&cxt);
    if (context == nullptr) return false;

    return context->genericBlockDisablerMode();
}


} // namespace adblock
