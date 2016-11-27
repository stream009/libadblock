#include "elem_hide_option.hpp"

#include "context.hpp"

namespace adblock {

bool ElemHideOption::
doMatch2(Uri const&, Context const&) const
{
    return false;
}

} // namespace adblock
