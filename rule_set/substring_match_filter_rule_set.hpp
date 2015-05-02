#ifndef ADBLOCK_SUBSTRING_MATCH_FILTER_RULE_SET_HPP
#define ADBLOCK_SUBSTRING_MATCH_FILTER_RULE_SET_HPP

#include "filter_rule_set.hpp"
#include "type.hpp"
#include "trie/trie.hpp"

namespace adblock {

class SubstringMatchFilterRuleSet : public FilterRuleSet
{
    using Base = FilterRuleSet;
public:
    using Base::FilterRulePtr;
    using Base::FilterRules;
    using Rules = trie::Trie<StringRange, FilterRulePtr>;

private:
    void doPut(const FilterRulePtr&) override;
    FilterRules doQuery(const Uri&) const override;
    void doStatistics(std::ostream&) const override;

private:
    Rules m_rules;
};

} // namespace adblock

#endif // ADBLOCK_SUBSTRING_MATCH_FILTER_RULE_SET_HPP
