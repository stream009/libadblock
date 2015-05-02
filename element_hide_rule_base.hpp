#ifndef ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP
#define ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP

#include "type.hpp"
#include "rule_set/domained_element_hide_rule_set.hpp"

#include <memory>
#include <string>
#include <vector>

#include <boost/container/flat_set.hpp>

namespace adblock {

class ElementHideRuleBase
{
public:
    using ElementHideRulePtr = std::shared_ptr<ElementHideRule>;
private:
    struct CompareBySelector {
        bool operator()(const ElementHideRulePtr &lhs,
                        const ElementHideRulePtr &rhs) const;
    };
public:
    using ElementHideRules = std::vector<ElementHideRulePtr>;

public:
    // query
    std::string query(const Uri&) const;
    void statistics(std::ostream&) const;

    // modifier
    void put(const ElementHideRulePtr&);

public:
    DomainedElementHideRuleSet m_domainedBlackList;
    DomainedElementHideRuleSet m_domainedWhiteList;
    ElementHideRules m_blackList;
};

} // namespace adblock

#endif // ADBLOCK_ELEMENT_HIDE_RULE_BASE_HPP
