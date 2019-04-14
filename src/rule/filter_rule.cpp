#include "filter_rule.hpp"

#include "context.hpp"
#include "option/collapse_option.hpp"
#include "option/csp_option.hpp"
#include "option/document_option.hpp"
#include "option/domain_data_base.hpp"
#include "option/domain_option.hpp"
#include "option/elem_hide_option.hpp"
#include "option/font_option.hpp"
#include "option/generic_block_option.hpp"
#include "option/generic_hide_option.hpp"
#include "option/image_option.hpp"
#include "option/match_case_option.hpp"
#include "option/media_option.hpp"
#include "option/object_option.hpp"
#include "option/object_subrequest_option.hpp"
#include "option/other_option.hpp"
#include "option/ping_option.hpp"
#include "option/popup_option.hpp"
#include "option/restriction_option.hpp"
#include "option/script_option.hpp"
#include "option/site_key_option.hpp"
#include "option/style_sheet_option.hpp"
#include "option/subdocument_option.hpp"
#include "option/third_party_option.hpp"
#include "option/web_rtc_option.hpp"
#include "option/web_socket_option.hpp"
#include "option/white_list_option.hpp"
#include "option/xml_http_request_option.hpp"
#include "pattern/pattern.hpp"
#include "white_list_query_context.hpp"

#include <cassert>
#include <ostream>

#include <boost/algorithm/string.hpp>

namespace adblock {

FilterRule::
FilterRule(std::unique_ptr<Pattern> pattern,
           OptionPtrs options)
    : m_pattern { std::move(pattern) }
{
    for (auto& opt: options) {
        // RequestType
        if (auto* const o = dynamic_cast<OtherOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::Inverse);
            }
            m_options.set(FilterOption::Other);
        }
        else if (auto* const o = dynamic_cast<ScriptOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::Inverse);
            }
            m_options.set(FilterOption::Script);
        }
        else if (auto* const o = dynamic_cast<StyleSheetOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::Inverse);
            }
            m_options.set(FilterOption::StyleSheet);
        }
        else if (auto* const o = dynamic_cast<ImageOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::Inverse);
            }
            m_options.set(FilterOption::Image);
        }
        else if (auto* const o = dynamic_cast<MediaOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::Inverse);
            }
            m_options.set(FilterOption::Media);
        }
        else if (auto* const o = dynamic_cast<FontOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::Inverse);
            }
            m_options.set(FilterOption::Font);
        }
        else if (auto* const o = dynamic_cast<ObjectOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::Inverse);
            }
            m_options.set(FilterOption::Object);
        }
        else if (auto* const o = dynamic_cast<SubDocumentOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::Inverse);
            }
            m_options.set(FilterOption::SubDocument);
        }
        else if (auto* const o = dynamic_cast<WebSocketOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::Inverse);
            }
            m_options.set(FilterOption::WebSocket);
        }
        else if (auto* const o = dynamic_cast<WebRtcOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::Inverse);
            }
            m_options.set(FilterOption::WebRtc);
        }
        else if (auto* const o = dynamic_cast<PingOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::Inverse);
            }
            m_options.set(FilterOption::Ping);
        }
        else if (auto* const o = dynamic_cast<XmlHttpRequestOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::Inverse);
            }
            m_options.set(FilterOption::XmlHttpRequest);
        }
        else if (auto* const o = dynamic_cast<ObjectSubRequestOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::Inverse);
            }
            m_options.set(FilterOption::ObjectSubRequest);
        }
        // QueryType
        else if (auto* const o = dynamic_cast<PopUpOption*>(opt.get())) {
            m_options.set(FilterOption::Popup);
        }
        else if (auto* const o = dynamic_cast<DocumentOption*>(opt.get())) {
            m_options.set(o->inverse() ?
                FilterOption::DocumentInv : FilterOption::Document);
        }
        else if (auto* const o = dynamic_cast<ElemHideOption*>(opt.get())) {
            m_options.set(o->inverse() ?
                FilterOption::ElemHideInv : FilterOption::ElemHide);
        }
        else if (auto* const o = dynamic_cast<GenericBlockOption*>(opt.get())) {
            m_options.set(FilterOption::GenericBlock);
        }
        else if (auto* const o = dynamic_cast<GenericHideOption*>(opt.get())) {
            m_options.set(FilterOption::GenericHide);
        }
        else if (auto* const o = dynamic_cast<CspOption*>(opt.get())) {
            m_options.set(FilterOption::Csp);
            m_cspValue = std::make_unique<StringRange>(o->policy());
        }
        // Restriction
        else if (auto* const o = dynamic_cast<DomainOption*>(opt.get())) {
            m_options.set(FilterOption::Domain);

            m_domains = std::make_unique<Domains>();

            for (auto& dom: o->includeDomains()) {
                m_domains->emplace_back(dom.begin(), dom.end());
            }

            for (auto& dom: o->excludeDomains()) {
                std::string d;
                d.push_back('~');
                d.append(dom.begin(), dom.end());

                m_domains->push_back(std::move(d));
            }
        }
        else if (auto* const o = dynamic_cast<SiteKeyOption*>(opt.get())) {
            m_options.set(FilterOption::SiteKey);

            m_siteKeys = std::make_unique<SiteKeys>();

            for (auto& key: o->siteKeys()) {
                m_siteKeys->push_back(key);
            }
        }
        else if (auto* const o = dynamic_cast<ThirdPartyOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::SameOrigin);
            }
            else {
                m_options.set(FilterOption::ThirdParty);
            }
        }
        // misc.
        else if (auto* const o = dynamic_cast<MatchCaseOption*>(opt.get())) {
            m_options.set(FilterOption::MatchCase);
        }
        else if (auto* const o = dynamic_cast<CollapseOption*>(opt.get())) {
            if (o->inverse()) {
                m_options.set(FilterOption::NeverCollapse);
            }
            else {
                m_options.set(FilterOption::AlwaysCollapse);
            }
        }
        else {
            assert(false);
        }
    }
}

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
            StringRange const d { &dom[1], &dom[dom.size()] };
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
