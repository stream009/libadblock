#ifndef ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_SET_HPP
#define ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_SET_HPP

#include "type.hpp"
#include "suffix_map.hpp"
#include "rule/element_hide_rule.hpp"

#include <memory>
#include <vector>

namespace adblock {

class DomainedElementHideRuleSet
{
public:
    using ElementHideRulePtr = std::shared_ptr<ElementHideRule>;
    using ElementHideRules = std::vector<ElementHideRulePtr>;
    using RuleMap = SuffixMap<StringRange, ElementHideRules>;

public:
    // query
    ElementHideRules query(const Uri&) const;

    // modifier
    void put(const ElementHideRulePtr&);

private:
    RuleMap m_normal;
    RuleMap m_exception;
};

} // namespace adblock

#endif // ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_SET_HPP
