#include "web_rtc_option.hpp"

#include "context.hpp"

namespace adblock {

bool WebRtcOption::
doMatch2(Uri const&, Context const& context) const
{
    return context.isWebRtc();
}

} // namespace adblock

