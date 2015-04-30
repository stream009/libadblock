#ifndef ADBLOCK_DOMAIN_MATCH_FILTER_RULE_SET_HPP
#define ADBLOCK_DOMAIN_MATCH_FILTER_RULE_SET_HPP

#include "filter_rule_set.hpp"
#include "prefix_map.hpp"
#include "type.hpp"

namespace adblock {

class DomainMatchFilterRuleSet : public FilterRuleSet
{
    using Base = FilterRuleSet;
public:
    using Base::FilterRulePtr;
    using Base::FilterRules;
    using RuleMap = PrefixMap<StringRange, FilterRules>;

private:
    void doPut(const FilterRulePtr&) override;
    FilterRules doQuery(const Uri&) const override;

private:
    RuleMap m_map;
};

} // namespace adblock

#endif // ADBLOCK_DOMAIN_MATCH_FILTER_RULE_SET_HPP
