#ifndef ADBLOCK_OPTION_SITE_KEY_OPTION_HPP
#define ADBLOCK_OPTION_SITE_KEY_OPTION_HPP

#include "type.hpp"
#include "option.hpp"
#include "restriction_option.hpp"

#include <vector>

#include <boost/range/iterator_range.hpp>

namespace adblock {

class Context;

class SiteKeyOption : public Option, public RestrictionOption
{
public:
    using SiteKeys = std::vector<StringRange>;
    using SiteKeysRange = boost::iterator_range<SiteKeys::const_iterator>;
public:
    SiteKeyOption(const SiteKeys &siteKeys)
        : m_siteKeys { siteKeys }
    {}

    SiteKeysRange siteKeys() const { return m_siteKeys; }

private:
    bool doMatch(const Uri&, const Context&) const override;

    void validate()
    {
        assert(!m_siteKeys.empty());
    }

private:
    SiteKeys m_siteKeys;
};

} // namespace adblock

#endif // ADBLOCK_OPTION_SITE_KEY_OPTION_HPP
