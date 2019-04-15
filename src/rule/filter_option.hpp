#ifndef ADBLOCK_RULE_FILTER_OPTION_HPP
#define ADBLOCK_RULE_FILTER_OPTION_HPP

#include <bitset>
#include <iosfwd>

namespace adblock {

class Context;
class WhiteListQueryContext;

enum class FilterOption : uint64_t {
    // request type
    Other = 0,
    OtherInv,
    Script,
    ScriptInv,
    StyleSheet,
    StyleSheetInv,
    Image,
    ImageInv,
    Media,
    MediaInv,
    Font,
    FontInv,
    Object,
    ObjectInv,
    SubDocument,
    SubDocumentInv,
    WebSocket,
    WebSocketInv,
    WebRtc,
    WebRtcInv,
    Ping,
    PingInv,
    XmlHttpRequest,
    XmlHttpRequestInv,
    ObjectSubRequest,
    ObjectSubRequestInv,
    // query type
    Popup,
    Document,
    DocumentInv,
    ElemHide,
    ElemHideInv,
    GenericBlock,
    GenericHide,
    Csp,
    Rewrite,
    // restriction
    Domain,
    SameOrigin,
    ThirdParty,
    SiteKey,
    //  misc.
    MatchCase,
    AlwaysCollapse,
    NeverCollapse,
};

class FilterOptionSet
{
    using BitSet = std::bitset<64>;
public:
    // query
    bool test(FilterOption) const;
    size_t count() const;

    bool typeSpecified() const;

    bool matchTypeOptions(Context const&) const;
    bool matchWhiteListOptions(WhiteListQueryContext const&) const;

    // modifier
    void set(FilterOption);

private:
    BitSet m_options;
};

std::ostream& operator<<(std::ostream&, FilterOptionSet const&);

} // namespace adblock

#endif // ADBLOCK_RULE_FILTER_OPTION_HPP
