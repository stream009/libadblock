#include "media_option.hpp"

#include "context.hpp"

namespace adblock {

bool MediaOption::
doMatch2(const Uri&, const Context &context) const
{
    return context.fromAudioVideoTag();
}

} // namespace adblock
