#include "filter_rule.hpp"

#include "domain_data_base.hpp"
#include "utility.hpp"

#include "core/context.hpp"
#include "core/uri.hpp"
#include "core/white_list_query_context.hpp"
#include "pattern/pattern.hpp"

#include <cassert>
#include <ostream>

namespace adblock {

FilterRule::
FilterRule(std::unique_ptr<Pattern> pattern,
           FilterOptionSet options,
           std::unique_ptr<Domains> domains,
           std::unique_ptr<SiteKeys> siteKeys,
           std::unique_ptr<StringRange> cspValue)
    : m_pattern { std::move(pattern) }
    , m_options { std::move(options) }
    , m_domains { std::move(domains) }
    , m_siteKeys { std::move(siteKeys) }
    , m_cspValue { std::move(cspValue) }
{}

FilterRule::~FilterRule() = default;

bool FilterRule::
match(Uri const& uri, Context const& cxt,
                      bool const specificOnly/*= false*/) const
{
    if (specificOnly && !domains()) return false;

    auto const matchCase = hasOption(FilterOption::MatchCase);

    if (!m_pattern->match(uri, matchCase)) return false;

    if (auto* const c = dynamic_cast<WhiteListQueryContext const*>(&cxt)) {
        return m_options.matchWhiteListOptions(*c)
            && checkRestrictions(uri, cxt);
    }
    else {
        return m_options.matchTypeOptions(cxt)
            && checkRestrictions(uri, cxt);
    }
}

Pattern const& FilterRule::
pattern() const
{
    return *m_pattern;
}

bool FilterRule::
hasOption(FilterOption const mask) const
{
    return m_options.test(mask);
}

size_t FilterRule::
numOptions() const
{
    return m_options.count();
}

bool FilterRule::
checkRestrictions(Uri const& uri, Context const& cxt) const
{
    return matchDomain(cxt)
        && matchSiteKey(cxt)
        && matchOrigin(uri, cxt);
}

bool FilterRule::
matchDomain(Context const& cxt) const
{
    auto const host = cxt.origin().host();

    return adblock::matchDomain(host, m_domains.get());
}

bool FilterRule::
matchSiteKey(Context const& cxt) const
{
    if (!m_siteKeys) return true;

    auto const& siteKey = cxt.siteKey();

    for (auto& key: *m_siteKeys) {
        // [[asserts: !key.empty()]]

        if (key == siteKey) return true;
    }

    return false;
}

bool FilterRule::
matchOrigin(Uri const& uri, Context const& cxt) const
{
    if (m_options.test(FilterOption::SameOrigin)) {

        auto const uriDomain = domain_db::getRegisteredDomain(uri);
        auto const originDomain = domain_db::getRegisteredDomain(cxt.origin());

        return uriDomain == originDomain;
    }
    else if (m_options.test(FilterOption::ThirdParty)) {

        auto const uriDomain = domain_db::getRegisteredDomain(uri);
        auto const originDomain = domain_db::getRegisteredDomain(cxt.origin());

        return uriDomain != originDomain;
    }
    else {
        return true;
    }
}

void FilterRule::
print(std::ostream &os) const
{
    os << "Pattern: " << *m_pattern << ", Option: " << m_options;
}

} // namespace adblock
