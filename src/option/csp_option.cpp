#include "csp_option.hpp"

#include "context.hpp"

#include <boost/algorithm/cxx11/any_of.hpp>

namespace adblock {

bool CspOption::
doMatch(Uri const&, Context const& cxt) const
{
    return cxt.isCsp();
}

} // namespace adblock
