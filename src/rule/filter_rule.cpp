#include "filter_rule.hpp"

#include "context.hpp"
#include "option/domain_data_base.hpp"
#include "pattern/pattern.hpp"
#include "white_list_query_context.hpp"

#include <cassert>
#include <ostream>

#include <boost/algorithm/string.hpp>

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
        return matchWhiteListOptions(*c)
            && matchRestrictionOptions(uri, cxt);
    }
    else {
        return matchTypeOptions(cxt)
            && matchRestrictionOptions(uri, cxt);
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
matchWhiteListOptions(WhiteListQueryContext const& cxt) const
{
    return (cxt.blockDisablerMode() && m_options.test(FilterOption::Document))
        || (cxt.hideDisablerMode() && m_options.test(FilterOption::ElemHide))
        || (cxt.genericBlockDisablerMode() && m_options.test(FilterOption::GenericBlock))
        || (cxt.genericHideDisablerMode() && m_options.test(FilterOption::GenericHide));
}

bool FilterRule::
matchTypeOptions(Context const& cxt) const
{
    if (!m_options.typeSpecified()) return true;

    bool const inverse = m_options.test(FilterOption::Inverse);

    if (cxt.fromScriptTag()) {
        return m_options.test(FilterOption::Script) ^ inverse;
    }
    else if (cxt.isExternalStyleSheet()) {
        return m_options.test(FilterOption::StyleSheet) ^ inverse;
    }
    else if (cxt.fromImageTag()) {
        return m_options.test(FilterOption::Image) ^ inverse;
    }
    else if (cxt.fromAudioVideoTag()) {
        return m_options.test(FilterOption::Media) ^ inverse;
    }
    else if (cxt.isFont()) {
        return m_options.test(FilterOption::Font) ^ inverse;
    }
    else if (cxt.fromObjectTag()) {
        return m_options.test(FilterOption::Object) ^ inverse;
    }
    else if (cxt.isSubDocument()) {
        return m_options.test(FilterOption::SubDocument) ^ inverse;
    }
    else if (cxt.isWebSocket()) {
        return m_options.test(FilterOption::WebSocket) ^ inverse;
    }
    else if (cxt.isWebRtc()) {
        return m_options.test(FilterOption::WebRtc) ^ inverse;
    }
    else if (cxt.isPing()) {
        return m_options.test(FilterOption::Ping) ^ inverse;
    }
    else if (cxt.fromXmlHttpRequest()) {
        return m_options.test(FilterOption::XmlHttpRequest) ^ inverse;
    }
    else if (cxt.fromPlugins()) {
        return m_options.test(FilterOption::ObjectSubRequest) ^ inverse;
    }
    else if (cxt.isCsp()) {
        return m_options.test(FilterOption::Csp); //TODO separate
    }
    else {
        return m_options.test(FilterOption::Other) ^ inverse;
    }
}

bool FilterRule::
matchRestrictionOptions(Uri const& uri, Context const& cxt) const
{
    return matchDomain(cxt)
        && matchSiteKey(cxt)
        && matchOrigin(uri, cxt);
}

bool FilterRule::
matchDomain(Context const& cxt) const
{
    namespace ba = boost::algorithm;

    if (!m_domains) return true;

    bool result = false;
    bool hasIncludeDomain = false;
    bool hasExcludeDomain = false;
    auto const& host = cxt.origin().host();

    for (auto& dom: *m_domains) {
        // [[asserts: !d.empty()]]

        if (dom[0] == '~') {
            hasExcludeDomain = true;
            StringRange const d { dom.begin() + 1, dom.end() };
            if (ba::ends_with(host, d)) {
                return false;
            }
        }
        else {
            hasIncludeDomain = true;
            if (ba::ends_with(host, dom)) {
                result = true;
            }
        }
    }

    if (!hasIncludeDomain && hasExcludeDomain) {
        return true;
    }
    else {
        return result;
    }
}

bool FilterRule::
matchSiteKey(Context const& cxt) const
{
    namespace ba = boost::algorithm;

    if (!m_siteKeys) return true;

    auto const& siteKey = cxt.siteKey();

    for (auto& key: *m_siteKeys) {
        // [[asserts: !key.empty()]]

        if (ba::equals(key, siteKey)) return true;
    }

    return false;
}

bool FilterRule::
matchOrigin(Uri const& uri, Context const& cxt) const
{
    namespace ba = boost::algorithm;

    if (m_options.test(FilterOption::SameOrigin)) {
        DomainDataBase ddb;

        const auto &uriDomain = ddb.query(uri);
        const auto &originDomain = ddb.query(cxt.origin());

        return boost::equals(uriDomain, originDomain);
    }
    else if (m_options.test(FilterOption::ThirdParty)) {
        DomainDataBase ddb;

        const auto &uriDomain = ddb.query(uri);
        const auto &originDomain = ddb.query(cxt.origin());

        return !ba::equals(uriDomain, originDomain);
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
