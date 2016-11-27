#include "other_option.hpp"

#include "context.hpp"

namespace adblock {

bool OtherOption::
doMatch2(const Uri&, const Context &context) const
{
    return !context.fromScriptTag()
        && !context.fromImageTag()
        && !context.isExternalStyleSheet()
        && !context.fromObjectTag()
        && !context.fromXmlHttpRequest()
        && !context.fromPlugins()
        && !context.isSubDocument()
        // According to the specification,(https://adblockplus.org/en/filters)
        // this option also has to exclude the document option and the
        // elemhide option.
        // But those two options are option to specify whitelist.
        // I don't know how to exclude them.
    ;
}

} // namespace adblock
