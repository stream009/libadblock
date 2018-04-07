#include "web_socket_option.hpp"

#include "context.hpp"

namespace adblock {

bool WebSocketOption::
doMatch2(const Uri&, const Context &context) const
{
    return context.isWebSocket();
}

} // namespace adblock
