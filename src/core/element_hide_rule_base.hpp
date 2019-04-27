#ifndef ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP
#define ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP

#include "core/json_fwd.hpp"
#include "index/domained_element_hide_rule_map.hpp"

#include <vector>

namespace adblock {

class FilterRuleBase;
class StringRange;
class Uri;

class ElementHideRuleBase
{
public:
    using ElementHideRules = std::vector<ElementHideRule const*>;

public:
    ElementHideRuleBase(FilterRuleBase const&);
    ~ElementHideRuleBase();

    // query
    ElementHideRules
        lookupRules(Uri const&, StringRange siteKey = {}) const;

    ElementHideRules
        lookupExtendedRules(Uri const&, StringRange siteKey = {}) const;

    json::object statistics() const;

    // modifier
    void put(ElementHideRule const&);
    void clear();

private:
    bool isWhiteListed(Uri const&, StringRange siteKey) const;
    bool isDomainSpecificOnly(Uri const&, StringRange siteKey) const;

public:
    DomainedElementHideRuleMap m_domainedBlackList;
    DomainedElementHideRuleMap m_domainedWhiteList;
    ElementHideRules m_genericBlackList;
    ElementHideRules m_genericWhiteList;

    DomainedElementHideRuleMap m_domainedExtendedBlackList;
    ElementHideRules m_genericExtendedBlackList;

    FilterRuleBase const& m_filterRuleBase;
};

} // namespace adblock

#endif // ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP
