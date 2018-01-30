#ifndef ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP
#define ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP

#include "type.hpp"
#include "filter_rule_group.hpp"
#include "rule_set/domained_element_hide_rule_set.hpp"

#include <string>
#include <vector>

#include <boost/property_tree/ptree_fwd.hpp>

namespace adblock {

class FilterRuleBase;

class ElementHideRuleBase
{
private:
    struct CompareBySelector {
        bool operator()(const ElementHideRule &lhs,
                        const ElementHideRule &rhs) const;
    };
public:
    using ElementHideRules = std::vector<const ElementHideRule*>;

public:
    ElementHideRuleBase(FilterRuleBase const&);

    // query
    std::string query(Uri const&, StringRange const& siteKey = {}) const;

    ElementHideRules
        lookupExtendedRule(Uri const&, StringRange const& siteKey = {}) const;

    boost::property_tree::ptree statistics() const;

    // modifier
    void put(const ElementHideRule&);
    void clear();

public:
    DomainedElementHideRuleSet m_domainedBlackList;
    DomainedElementHideRuleSet m_domainedWhiteList;
    ElementHideRules m_genericBlackList;
    ElementHideRules m_genericWhiteList;

    DomainedElementHideRuleSet m_domainedExtendedBlackList;
    ElementHideRules m_genericExtendedBlackList;

    FilterRuleBase const& m_filterRuleBase;
};

} // namespace adblock

#endif // ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP
