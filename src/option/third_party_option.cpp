#include "third_party_option.hpp"

#include "context.hpp"
#include "domain_data_base.hpp"

#include <boost/algorithm/string.hpp>

namespace adblock {

bool ThirdPartyOption::
doMatch2(const Uri &uri, const Context &context) const
{
    DomainDataBase ddb;

    const auto &uriDomain = ddb.query(uri);
    const auto &originDomain = ddb.query(context.origin());

    return !boost::equals(uriDomain, originDomain);
}

} // namespace adblock
