#ifndef ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP
#define ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP

#include "index/domained_element_hide_rule_map.hpp"

#include <string>
#include <vector>

#include <boost/property_tree/ptree_fwd.hpp>

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

    // query
    std::string query(Uri const&, StringRange const& siteKey = {}) const;

    ElementHideRules
        lookupExtendedRule(Uri const&, StringRange const& siteKey = {}) const;

    boost::property_tree::ptree statistics() const;

    // modifier
    void put(ElementHideRule const&);
    void clear();

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
