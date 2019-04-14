#ifndef ADBLOCK_RULE_FILTER_OPTION_HPP
#define ADBLOCK_RULE_FILTER_OPTION_HPP

#include <bitset>

namespace adblock {

enum FilterOption {
    // request type
    Other = 0,
    Script,
    StyleSheet,
    Image,
    Media,
    Font,
    Object,
    SubDocument,
    WebSocket,
    WebRtc,
    Ping,
    XmlHttpRequest,
    ObjectSubRequest,
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
    Inverse,
    MatchCase,
    AlwaysCollapse,
    NeverCollapse,
};

using FilterOptionSet = std::bitset<64>;

} // namespace adblock

#endif // ADBLOCK_RULE_FILTER_OPTION_HPP
