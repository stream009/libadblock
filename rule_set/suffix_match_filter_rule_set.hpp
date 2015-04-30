#ifndef ADBLOCK_SUFFIX_MATCH_FILTER_RULE_SET_HPP
#define ADBLOCK_SUFFIX_MATCH_FILTER_RULE_SET_HPP

#include "filter_rule_set.hpp"
#include "suffix_map.hpp"
#include "type.hpp"

namespace adblock {

class SuffixMatchFilterRuleSet : public FilterRuleSet
{
    using Base = FilterRuleSet;
public:
    using Base::FilterRulePtr;
    using Base::FilterRules;
    using RuleMap = SuffixMap<StringRange, FilterRules>;

private:
    void doPut(const FilterRulePtr&) override;
    FilterRules doQuery(const Uri&) const override;

private:
    RuleMap m_map;
};

} // namespace adblock

#endif // ADBLOCK_SUFFIX_MATCH_FILTER_RULE_SET_HPP
