#include "site_key_option.hpp"

#include "context.hpp"

#include <boost/algorithm/cxx11/any_of.hpp>

namespace adblock {

bool SiteKeyOption::
doMatch(const Uri&, const Context &context) const
{
    const auto &theSiteKey = context.siteKey();

    namespace ba = boost::algorithm;
    return ba::any_of(m_siteKeys,
        [&theSiteKey] (const StringRange &aSiteKey) {
            return ba::equals(theSiteKey, aSiteKey);
        }
    );
}

} // namespace adblock
