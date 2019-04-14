#ifndef ADBLOCK_RULE_FILTER_OPTION_HPP
#define ADBLOCK_RULE_FILTER_OPTION_HPP

#include "option.hpp"
#include "type.hpp"

#include <bitset>
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
        Other,
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

public:
    FilterOption(OptionPtrs&&);

    // accessor
    Domains* domains() const { return m_domains.get(); }
    SiteKeys* siteKeys() const { return m_siteKeys.get(); }
    StringRange* cspValue() const { return m_cspValue.get(); }

    // query
    bool match(Uri const&, Context const&) const;

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
    std::bitset<64> m_options;
    std::unique_ptr<Domains> m_domains;
    std::unique_ptr<SiteKeys> m_siteKeys;
    std::unique_ptr<StringRange> m_cspValue;
};

} // namespace adblock

#endif // ADBLOCK_RULE_FILTER_OPTION_HPP
