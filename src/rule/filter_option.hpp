#ifndef ADBLOCK_RULE_FILTER_OPTION_HPP
#define ADBLOCK_RULE_FILTER_OPTION_HPP

#include <bitset>
#include <iosfwd>

namespace adblock {

enum class FilterOption : uint64_t {
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

class FilterOptionSet
{
    using BitSet = std::bitset<64>;
public:
    // query
    bool test(FilterOption) const;
    size_t count() const;
    bool any() const;
    bool typeSpecified() const;

    // modifier
    void set(FilterOption);

private:
    BitSet m_options;
};

std::ostream& operator<<(std::ostream&, FilterOptionSet const&);

} // namespace adblock

#endif // ADBLOCK_RULE_FILTER_OPTION_HPP
