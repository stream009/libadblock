#include "filter_option.hpp"

#include "context.hpp"
#include "option/domain_data_base.hpp"
#include "option/other_option.hpp"
#include "option/script_option.hpp"
#include "option/style_sheet_option.hpp"
#include "option/image_option.hpp"
#include "option/media_option.hpp"
#include "option/font_option.hpp"
#include "option/object_option.hpp"
#include "option/subdocument_option.hpp"
#include "option/web_socket_option.hpp"
#include "option/web_rtc_option.hpp"
#include "option/ping_option.hpp"
#include "option/xml_http_request_option.hpp"
#include "option/object_subrequest_option.hpp"
#include "option/popup_option.hpp"
#include "option/document_option.hpp"
#include "option/elem_hide_option.hpp"
#include "option/generic_block_option.hpp"
#include "option/generic_hide_option.hpp"
#include "option/domain_option.hpp"
#include "option/site_key_option.hpp"
#include "option/third_party_option.hpp"
#include "option/match_case_option.hpp"
#include "option/collapse_option.hpp"
#include "option/csp_option.hpp"
#include "white_list_query_context.hpp"

#include <cassert>
#include <iostream> //TODO

#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/string.hpp>

namespace adblock {

FilterOption::
FilterOption(OptionPtrs&& options)
{
    for (auto& opt: options) {
        // RequestType
        if (auto* const o = dynamic_cast<OtherOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= Inverse;
            }
            m_flags |= Other;
        }
        else if (auto* const o = dynamic_cast<ScriptOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= Inverse;
            }
            m_flags |= Script;
        }
        else if (auto* const o = dynamic_cast<StyleSheetOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= Inverse;
            }
            m_flags |= StyleSheet;
        }
        else if (auto* const o = dynamic_cast<ImageOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= Inverse;
            }
            m_flags |= Image;
        }
        else if (auto* const o = dynamic_cast<MediaOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= Inverse;
            }
            m_flags |= Media;
        }
        else if (auto* const o = dynamic_cast<FontOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= Inverse;
            }
            m_flags |= Font;
        }
        else if (auto* const o = dynamic_cast<ObjectOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= Inverse;
            }
            m_flags |= Object;
        }
        else if (auto* const o = dynamic_cast<SubDocumentOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= Inverse;
            }
            m_flags |= SubDocument;
        }
        else if (auto* const o = dynamic_cast<WebSocketOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= Inverse;
            }
            m_flags |= WebSocket;
        }
        else if (auto* const o = dynamic_cast<WebRtcOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= Inverse;
            }
            m_flags |= WebRtc;
        }
        else if (auto* const o = dynamic_cast<PingOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= Inverse;
            }
            m_flags |= Ping;
        }
        else if (auto* const o = dynamic_cast<XmlHttpRequestOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= Inverse;
            }
            m_flags |= XmlHttpRequest;
        }
        else if (auto* const o = dynamic_cast<ObjectSubRequestOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= Inverse;
            }
            m_flags |= ObjectSubRequest;
        }
        // QueryType
        else if (auto* const o = dynamic_cast<PopUpOption*>(opt.get())) {
            m_flags |= Popup;
        }
        else if (auto* const o = dynamic_cast<DocumentOption*>(opt.get())) {
            m_flags |= (o->inverse() ? DocumentInv : Document);
        }
        else if (auto* const o = dynamic_cast<ElemHideOption*>(opt.get())) {
            m_flags |= (o->inverse() ? ElemHideInv : ElemHide);
        }
        else if (auto* const o = dynamic_cast<GenericBlockOption*>(opt.get())) {
            m_flags |= GenericBlock;
        }
        else if (auto* const o = dynamic_cast<GenericHideOption*>(opt.get())) {
            m_flags |= GenericHide;
        }
        else if (auto* const o = dynamic_cast<CspOption*>(opt.get())) {
            m_flags |= Csp;
            m_cspValue = std::make_unique<StringRange>(o->policy());
        }
        // Restriction
        else if (auto* const o = dynamic_cast<DomainOption*>(opt.get())) {
            m_flags |= Domain;

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
            m_flags |= SiteKey;

            m_siteKeys = std::make_unique<SiteKeys>();

            for (auto& key: o->siteKeys()) {
                m_siteKeys->push_back(key);
            }
        }
        else if (auto* const o = dynamic_cast<ThirdPartyOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= SameOrigin;
            }
            else {
                m_flags |= ThirdParty;
            }
        }
        // misc.
        else if (auto* const o = dynamic_cast<MatchCaseOption*>(opt.get())) {
            m_flags |= MatchCase;
        }
        else if (auto* const o = dynamic_cast<CollapseOption*>(opt.get())) {
            if (o->inverse()) {
                m_flags |= NeverCollapse;
            }
            else {
                m_flags |= AlwaysCollapse;
            }
        }
        else {
            std::cout << typeid(*opt).name() << '\n';
            assert(false);
        }
    }
}

bool FilterOption::
match(Uri const& uri, Context const& cxt) const
{
    if (auto* const c = dynamic_cast<WhiteListQueryContext const*>(&cxt)) {
        return matchWhiteListOptions(*c)
            && matchRestrictionOptions(uri, cxt);
    }
    else {
        return matchTypeOptions(cxt)
            && matchRestrictionOptions(uri, cxt);
    }
}

bool FilterOption::
hasOption(uint32_t const mask) const
{
    return m_flags & mask;
}

size_t FilterOption::
numOptions() const
{
    size_t result = 0;

    for (size_t i = 0; i < 32; ++i) {
        if (m_flags & (1u << i)) ++result;
    }

    return result;
}

bool FilterOption::
matchWhiteListOptions(WhiteListQueryContext const& cxt) const
{
    return (cxt.blockDisablerMode() && (m_flags & Document))
        || (cxt.hideDisablerMode() && (m_flags & ElemHide))
        || (cxt.genericBlockDisablerMode() && (m_flags & GenericBlock))
        || (cxt.genericHideDisablerMode() && (m_flags & GenericHide));
}

bool FilterOption::
matchTypeOptions(Context const& cxt) const
{
    if (!typeSpecified()) return true;

    auto test = [&](auto flag) {
        return static_cast<bool>(m_flags & flag);
    };

    bool const inverse = test(Inverse);

    if (cxt.fromScriptTag()) {
        return test(Script) ^ inverse;
    }
    else if (cxt.isExternalStyleSheet()) {
        return test(StyleSheet) ^ inverse;
    }
    else if (cxt.fromImageTag()) {
        return test(Image) ^ inverse;
    }
    else if (cxt.fromAudioVideoTag()) {
        return test(Media) ^ inverse;
    }
    else if (cxt.isFont()) {
        return test(Font) ^ inverse;
    }
    else if (cxt.fromObjectTag()) {
        return test(Object) ^ inverse;
    }
    else if (cxt.isSubDocument()) {
        return test(SubDocument) ^ inverse;
    }
    else if (cxt.isWebSocket()) {
        return test(WebSocket) ^ inverse;
    }
    else if (cxt.isWebRtc()) {
        return test(WebRtc) ^ inverse;
    }
    else if (cxt.isPing()) {
        return test(Ping) ^ inverse;
    }
    else if (cxt.fromXmlHttpRequest()) {
        return test(XmlHttpRequest) ^ inverse;
    }
    else if (cxt.fromPlugins()) {
        return test(ObjectSubRequest) ^ inverse;
    }
    else if (cxt.isCsp()) {
        return test(Csp); //TODO separate
    }
    else {
        return test(Other) ^ inverse;
    }
}

bool FilterOption::
matchRestrictionOptions(Uri const& uri, Context const& cxt) const
{
    namespace ba = boost::algorithm;

    return matchDomain(cxt)
        && matchSiteKey(cxt)
        && matchOrigin(uri, cxt);
}

bool FilterOption::
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

bool FilterOption::
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

bool FilterOption::
matchOrigin(Uri const& uri, Context const& cxt) const
{
    namespace ba = boost::algorithm;

    if (m_flags & SameOrigin) {
        DomainDataBase ddb;

        const auto &uriDomain = ddb.query(uri);
        const auto &originDomain = ddb.query(cxt.origin());

        return boost::equals(uriDomain, originDomain);
    }
    else if (m_flags & ThirdParty) {
        DomainDataBase ddb;

        const auto &uriDomain = ddb.query(uri);
        const auto &originDomain = ddb.query(cxt.origin());

        return !boost::equals(uriDomain, originDomain);
    }
    else {
        return true;
    }
}

bool FilterOption::
typeSpecified() const
{
    constexpr auto typeOptions =
          Object
        | Script
        | StyleSheet
        | Image
        | Media
        | Font
        | Object
        | SubDocument
        | WebSocket
        | WebRtc
        | Ping
        | XmlHttpRequest
        | ObjectSubRequest;

    constexpr auto queryOptions =
          Popup
        | Document
        | DocumentInv
        | ElemHide
        | ElemHideInv
        | GenericBlock
        | Csp
        | Rewrite;

    return m_flags & (typeOptions | queryOptions);
}

} // namespace adblock
