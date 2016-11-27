#include "xml_http_request_option.hpp"

#include "context.hpp"

namespace adblock {

bool XmlHttpRequestOption::
doMatch2(const Uri&, const Context &context) const
{
    return context.fromXmlHttpRequest();
}

} // namespace adblock
