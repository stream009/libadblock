#include "document_option.hpp"

#include "context.hpp"

namespace adblock {

bool DocumentOption::
doMatch2(Uri const&, Context const&) const
{
    return false;
}

} // namespace adblock
