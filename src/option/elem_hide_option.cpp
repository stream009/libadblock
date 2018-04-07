#include "elem_hide_option.hpp"

#include "context.hpp"
#include "white_list_query_context.hpp"

namespace adblock {

bool ElemHideOption::
doMatch2(Uri const&, Context const& cxt) const
{
    // ignore inversed option
    if (this->inverse()) return true; // this true will turn into false

    auto* const context = dynamic_cast<WhiteListQueryContext const*>(&cxt);
    if (context == nullptr) return false;

    return context->hideDisablerMode();
}

} // namespace adblock
