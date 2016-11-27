#include "invertible_option.hpp"

#include "context.hpp"

namespace adblock {

bool InvertibleOption::
doMatch(const Uri &uri, const Context &context) const
{
    return m_inverse ? !doMatch2(uri, context)
                     :  doMatch2(uri, context);
}

bool InvertibleOption::
doMatch2(const Uri &uri, const Context &context) const
{
    return Option::doMatch(uri, context);
}

} // namespace adblock
