#ifndef ADBLOCK_PREFIX_MATCH_FILTER_RULE_SET_HPP
#define ADBLOCK_PREFIX_MATCH_FILTER_RULE_SET_HPP

#include "config.hpp"
#include "filter_rule_set.hpp"
#include "type.hpp"

#include <iosfwd>

namespace adblock {

class PrefixMatchFilterRuleSet : public FilterRuleSet
{
    using Base = FilterRuleSet;
public:
    using Base::FilterRules;
    using Rules = rule_set::Rules<StringRange, const FilterRule*>;

private:
    void doPut(const FilterRule&) override;
    FilterRules doQuery(const Uri&) const override;
    void doStatistics(std::ostream&) const override;

private:
    Rules m_rules;
};

} // namespace adblock

#endif // ADBLOCK_PREFIX_MATCH_FILTER_RULE_SET_HPP
