#ifndef ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP
#define ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP

#include "type.hpp"
#include "filter_rule_group.hpp"
#include "rule_set/domained_element_hide_rule_set.hpp"

#include <string>
#include <vector>

#include <boost/property_tree/ptree_fwd.hpp>

namespace adblock {

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
    // query
    std::string query(const Uri&) const;
    boost::property_tree::ptree statistics() const;

    // modifier
    void put(const ElementHideRule&);
    void putGenericDisablerRule(const FilterRule&);
    void clear();

private:
    bool genericDisabled(const Uri&) const;

public:
    DomainedElementHideRuleSet m_domainedBlackList;
    DomainedElementHideRuleSet m_domainedWhiteList;
    ElementHideRules m_blackList;
    FilterRuleGroup m_genericDisabled;
};

} // namespace adblock

#endif // ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP
