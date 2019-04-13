#ifndef ADBLOCK_RULE_FILTER_OPTION_HPP
#define ADBLOCK_RULE_FILTER_OPTION_HPP

#include "option.hpp"
#include "type.hpp"

#include <memory>
#include <string>
#include <vector>

namespace adblock {

class Context;
class WhiteListQueryContext;

class FilterOption
{
public:
    using OptionPtr = std::unique_ptr<Option>;
    using OptionPtrs = std::vector<OptionPtr>;
    using Domains = std::vector<std::string>;
    using SiteKeys = std::vector<StringRange>;

    enum : uint32_t {
        // request type
        Other = 1 << 0,
        Script = 1 << 1,
        StyleSheet = 1 << 2,
        Image = 1 << 3,
        Media = 1 << 4,
        Font = 1 << 5,
        Object = 1 << 6,
        SubDocument = 1 << 7,
        WebSocket = 1 << 8,
        WebRtc = 1 << 9,
        Ping = 1 << 10,
        XmlHttpRequest = 1 << 11,
        ObjectSubRequest = 1 << 12,
        // query type
        Popup = 1 << 13,
        Document = 1 << 14,
        DocumentInv = 1 << 15,
        ElemHide = 1 << 16,
        ElemHideInv = 1 << 17,
        GenericBlock = 1 << 18,
        GenericHide = 1 << 19,
        Csp = 1 << 20,
        Rewrite = 1 << 21,
        // restriction
        Domain = 1 << 22,
        SameOrigin = 1 << 23,
        ThirdParty = 1 << 24,
        SiteKey = 1 << 25,
        //  misc.
        Inverse = 1 << 26,
        MatchCase = 1 << 27,
        AlwaysCollapse = 1 << 28,
        NeverCollapse = 1 << 29,
    };

public:
    FilterOption(OptionPtrs&&);

    // accessor
    Domains* domains() const { return m_domains.get(); }
    SiteKeys* siteKeys() const { return m_siteKeys.get(); }
    StringRange* cspValue() const { return m_cspValue.get(); }

    // query
    bool match(Uri const&, Context const&) const;

    bool matchCase() const { return m_flags & MatchCase; }
    bool hasOption(uint32_t mask) const;
    size_t numOptions() const;

private:
    bool matchTypeOptions(Context const&) const;
    bool matchWhiteListOptions(WhiteListQueryContext const&) const;
    bool matchRestrictionOptions(Uri const&, Context const&) const;
    bool matchDomain(Context const&) const;
    bool matchSiteKey(Context const&) const;
    bool matchOrigin(Uri const&, Context const&) const;
    bool typeSpecified() const;

private:
    uint32_t m_flags = 0;

    std::unique_ptr<Domains> m_domains;
    std::unique_ptr<SiteKeys> m_siteKeys;
    std::unique_ptr<StringRange> m_cspValue;
};

} // namespace adblock

#endif // ADBLOCK_RULE_FILTER_OPTION_HPP
