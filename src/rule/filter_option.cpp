#include "filter_option.hpp"

#include "context.hpp"
#include "white_list_query_context.hpp"

#include <ostream>

namespace adblock {

bool FilterOptionSet::
test(FilterOption const opt) const
{
    return m_options.test(
        static_cast<size_t>(opt)
    );
}

size_t FilterOptionSet::
count() const
{
    return m_options.count();
}

static uint64_t
operator|(uint64_t const lhs, FilterOption const rhs)
{
    return lhs | (1ull << static_cast<size_t>(rhs));
}

static uint64_t
operator|(FilterOption const lhs, FilterOption const rhs)
{
    return (1ull << static_cast<size_t>(lhs))
         | (1ull << static_cast<size_t>(rhs));
}

bool FilterOptionSet::
typeSpecified() const
{
    static BitSet const mask {
        // request type
          FilterOption::Other
        | FilterOption::OtherInv
        | FilterOption::Script
        | FilterOption::ScriptInv
        | FilterOption::StyleSheet
        | FilterOption::StyleSheetInv
        | FilterOption::Image
        | FilterOption::ImageInv
        | FilterOption::Media
        | FilterOption::MediaInv
        | FilterOption::Font
        | FilterOption::FontInv
        | FilterOption::Object
        | FilterOption::ObjectInv
        | FilterOption::SubDocument
        | FilterOption::SubDocumentInv
        | FilterOption::WebSocket
        | FilterOption::WebSocketInv
        | FilterOption::WebRtc
        | FilterOption::WebRtcInv
        | FilterOption::Ping
        | FilterOption::PingInv
        | FilterOption::XmlHttpRequest
        | FilterOption::XmlHttpRequestInv
        | FilterOption::ObjectSubRequest
        | FilterOption::ObjectSubRequestInv
        // query type
        | FilterOption::Popup
        | FilterOption::Document
        | FilterOption::DocumentInv
        | FilterOption::ElemHide
        | FilterOption::ElemHideInv
        | FilterOption::GenericBlock
        | FilterOption::GenericHide
        | FilterOption::Csp
        | FilterOption::Rewrite
    };

    return (m_options & mask).any();
}

bool FilterOptionSet::
matchTypeOptions(Context const& cxt) const
{
    if (!typeSpecified()) return true;

    if (test(FilterOption::Script)) {
        if (cxt.fromScriptTag()) return true;
    }
    if (test(FilterOption::ScriptInv)) {
        if (!cxt.fromScriptTag()) return true;
    }
    if (test(FilterOption::StyleSheet)) {
        if (cxt.isExternalStyleSheet()) return true;
    }
    if (test(FilterOption::StyleSheetInv)) {
        if (!cxt.isExternalStyleSheet()) return true;
    }
    if (test(FilterOption::Image)) {
        if (cxt.fromImageTag()) return true;
    }
    if (test(FilterOption::ImageInv)) {
        if (!cxt.fromImageTag()) return true;
    }
    if (test(FilterOption::Media)) {
        if (cxt.fromAudioVideoTag()) return true;
    }
    if (test(FilterOption::MediaInv)) {
        if (!cxt.fromAudioVideoTag()) return true;
    }
    if (test(FilterOption::Font)) {
        if (cxt.isFont()) return true;
    }
    if (test(FilterOption::FontInv)) {
        if (!cxt.isFont()) return true;
    }
    if (test(FilterOption::Object)) {
        if (cxt.fromObjectTag()) return true;
    }
    if (test(FilterOption::ObjectInv)) {
        if (!cxt.fromObjectTag()) return true;
    }
    if (test(FilterOption::SubDocument)) {
        if (cxt.isSubDocument()) return true;
    }
    if (test(FilterOption::SubDocumentInv)) {
        if (!cxt.isSubDocument()) return true;
    }
    if (test(FilterOption::WebSocket)) {
        if (cxt.isWebSocket()) return true;
    }
    if (test(FilterOption::WebSocketInv)) {
        if (!cxt.isWebSocket()) return true;
    }
    if (test(FilterOption::WebRtc)) {
        if (cxt.isWebRtc()) return true;
    }
    if (test(FilterOption::WebRtcInv)) {
        if (!cxt.isWebRtc()) return true;
    }
    if (test(FilterOption::Ping)) {
        if (cxt.isPing()) return true;
    }
    if (test(FilterOption::PingInv)) {
        if (!cxt.isPing()) return true;
    }
    if (test(FilterOption::XmlHttpRequest)) {
        if (cxt.fromXmlHttpRequest()) return true;
    }
    if (test(FilterOption::XmlHttpRequestInv)) {
        if (!cxt.fromXmlHttpRequest()) return true;
    }
    if (test(FilterOption::ObjectSubRequest)) {
        if (cxt.fromPlugins()) return true;
    }
    if (test(FilterOption::ObjectSubRequestInv)) {
        if (!cxt.fromPlugins()) return true;
    }
    if (test(FilterOption::Csp)) { //TODO separate
        if (cxt.isCsp()) return true;
    }
    if (test(FilterOption::Popup)) { //TODO separate
        if (cxt.isPopUp()) return true;
    }
    if (test(FilterOption::Other)) {
        if (   !cxt.fromScriptTag()
            && !cxt.isExternalStyleSheet()
            && !cxt.fromImageTag()
            && !cxt.fromAudioVideoTag()
            && !cxt.isFont()
            && !cxt.fromObjectTag()
            && !cxt.isSubDocument()
            && !cxt.isWebSocket()
            && !cxt.isWebRtc()
            && !cxt.isPing()
            && !cxt.fromXmlHttpRequest()
            && !cxt.fromPlugins())
        {
            return true;
        }
    }
    if (test(FilterOption::OtherInv)) {
        if (   cxt.fromScriptTag()
            || cxt.isExternalStyleSheet()
            || cxt.fromImageTag()
            || cxt.fromAudioVideoTag()
            || cxt.isFont()
            || cxt.fromObjectTag()
            || cxt.isSubDocument()
            || cxt.isWebSocket()
            || cxt.isWebRtc()
            || cxt.isPing()
            || cxt.fromXmlHttpRequest()
            || cxt.fromPlugins())
        {
            return true;
        }
    }

    return false;
}

bool FilterOptionSet::
matchWhiteListOptions(WhiteListQueryContext const& cxt) const
{
    return (cxt.blockDisablerMode() && test(FilterOption::Document))
        || (cxt.hideDisablerMode() && test(FilterOption::ElemHide))
        || (cxt.genericBlockDisablerMode() && test(FilterOption::GenericBlock))
        || (cxt.genericHideDisablerMode() && test(FilterOption::GenericHide));
}

void FilterOptionSet::
set(FilterOption const opt)
{
    m_options.set(
        static_cast<size_t>(opt)
    );
}

std::ostream&
operator<<(std::ostream& os, FilterOptionSet const& options)
{
    os << "[";

    if (options.test(FilterOption::Other)) {
        os << "Other, ";
    }
    if (options.test(FilterOption::OtherInv)) {
        os << "OtherInv, ";
    }
    if (options.test(FilterOption::Script)) {
        os << "Script, ";
    }
    if (options.test(FilterOption::ScriptInv)) {
        os << "ScriptInv, ";
    }
    if (options.test(FilterOption::StyleSheet)) {
        os << "StyleSheet, ";
    }
    if (options.test(FilterOption::StyleSheetInv)) {
        os << "StyleSheetInv, ";
    }
    if (options.test(FilterOption::Image)) {
        os << "Image, ";
    }
    if (options.test(FilterOption::ImageInv)) {
        os << "ImageInv, ";
    }
    if (options.test(FilterOption::Media)) {
        os << "Media, ";
    }
    if (options.test(FilterOption::MediaInv)) {
        os << "MediaInv, ";
    }
    if (options.test(FilterOption::Font)) {
        os << "Font, ";
    }
    if (options.test(FilterOption::FontInv)) {
        os << "FontInv, ";
    }
    if (options.test(FilterOption::Object)) {
        os << "Object, ";
    }
    if (options.test(FilterOption::ObjectInv)) {
        os << "ObjectInv, ";
    }
    if (options.test(FilterOption::SubDocument)) {
        os << "SubDocument, ";
    }
    if (options.test(FilterOption::SubDocumentInv)) {
        os << "SubDocumentInv, ";
    }
    if (options.test(FilterOption::WebSocket)) {
        os << "WebSocket, ";
    }
    if (options.test(FilterOption::WebSocketInv)) {
        os << "WebSocketInv, ";
    }
    if (options.test(FilterOption::WebRtc)) {
        os << "WebRtc, ";
    }
    if (options.test(FilterOption::WebRtcInv)) {
        os << "WebRtcInv, ";
    }
    if (options.test(FilterOption::Ping)) {
        os << "Ping, ";
    }
    if (options.test(FilterOption::PingInv)) {
        os << "PingInv, ";
    }
    if (options.test(FilterOption::XmlHttpRequest)) {
        os << "XmlHttpRequest, ";
    }
    if (options.test(FilterOption::XmlHttpRequestInv)) {
        os << "XmlHttpRequestInv, ";
    }
    if (options.test(FilterOption::ObjectSubRequest)) {
        os << "ObjectSubRequest, ";
    }
    if (options.test(FilterOption::ObjectSubRequestInv)) {
        os << "ObjectSubRequestInv, ";
    }
    if (options.test(FilterOption::Popup)) {
        os << "Popup, ";
    }
    if (options.test(FilterOption::Document)) {
        os << "Document, ";
    }
    if (options.test(FilterOption::DocumentInv)) {
        os << "DocumentInv, ";
    }
    if (options.test(FilterOption::ElemHide)) {
        os << "ElemHide, ";
    }
    if (options.test(FilterOption::ElemHideInv)) {
        os << "ElemHideInv, ";
    }
    if (options.test(FilterOption::GenericBlock)) {
        os << "GenericBlock, ";
    }
    if (options.test(FilterOption::GenericHide)) {
        os << "GenericHide, ";
    }
    if (options.test(FilterOption::Csp)) {
        os << "Csp, ";
    }
    if (options.test(FilterOption::Rewrite)) {
        os << "Rewrite, ";
    }
    if (options.test(FilterOption::Domain)) {
        os << "Domain, ";
    }
    if (options.test(FilterOption::SameOrigin)) {
        os << "SameOrigin, ";
    }
    if (options.test(FilterOption::ThirdParty)) {
        os << "ThirdParty, ";
    }
    if (options.test(FilterOption::SiteKey)) {
        os << "SiteKey, ";
    }
    if (options.test(FilterOption::MatchCase)) {
        os << "MatchCase, ";
    }
    if (options.test(FilterOption::AlwaysCollapse)) {
        os << "AlwaysCollapse, ";
    }
    if (options.test(FilterOption::NeverCollapse)) {
        os << "NeverCollapse, ";
    }

    os << "]";

    return os;
}

} // namespace adblock
