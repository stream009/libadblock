#include "document_option.hpp"

#include "context.hpp"
#include "white_list_query_context.hpp"

namespace adblock {

bool DocumentOption::
doMatch2(Uri const&, Context const& cxt) const
{
    if (this->inverse()) return false;

    auto* const context = dynamic_cast<WhiteListQueryContext const*>(&cxt);
    if (context == nullptr) return false;

    return context->blockDisablerMode();
}

} // namespace adblock
