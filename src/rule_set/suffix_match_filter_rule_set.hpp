#ifndef ADBLOCK_SUFFIX_MATCH_FILTER_RULE_SET_HPP
#define ADBLOCK_SUFFIX_MATCH_FILTER_RULE_SET_HPP

#include "config.hpp"
#include "filter_rule_set.hpp"
#include "type.hpp"

#include <iterator>

#include <boost/property_tree/ptree_fwd.hpp>

namespace adblock {

class SuffixMatchFilterRuleSet : public FilterRuleSet
{
    using Base = FilterRuleSet;
public:
    using Base::FilterRules;
    using ReverseStringRange =
        boost::iterator_range<std::reverse_iterator<const char*>>;
    using Rules = rule_set::Rules<ReverseStringRange, const FilterRule*>;

private:
    void doPut(const FilterRule&) override;
    FilterRules doQuery(const Uri&) const override;
    boost::property_tree::ptree doStatistics() const override;
    void doClear() override;

private:
    Rules m_rules;
};

} // namespace adblock

#endif // ADBLOCK_SUFFIX_MATCH_FILTER_RULE_SET_HPP
