#include "subdocument_option.hpp"

#include "context.hpp"

namespace adblock {

bool SubDocumentOption::
doMatch2(const Uri&, const Context &context) const
{
    return context.isSubDocument();
}

} // namespace adblock
