#include "script_option.hpp"

#include "context.hpp"

namespace adblock {

bool ScriptOption::
doMatch2(const Uri&, const Context &context) const
{
    return context.fromScriptTag();
}

} // namespace adblock
