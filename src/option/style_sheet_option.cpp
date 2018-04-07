#include "style_sheet_option.hpp"

#include "context.hpp"

namespace adblock {

bool StyleSheetOption::
doMatch2(const Uri&, const Context &context) const
{
    return context.isExternalStyleSheet();
}

} // namespace adblock
