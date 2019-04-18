#ifndef ADBLOCK_DOMAIN_MATCH_FILTER_RULE_SET_HPP
#define ADBLOCK_DOMAIN_MATCH_FILTER_RULE_SET_HPP

#include "config.hpp"
#include "filter_rule_set.hpp"
#include "type.hpp"

#include <boost/property_tree/ptree_fwd.hpp>

namespace adblock {

class DomainMatchFilterRuleSet : public FilterRuleSet
{
    using Base = FilterRuleSet;
public:
    using Base::FilterRules;
    using Rules = rule_set::Rules<StringRange, FilterRule const*>;

private:
    // @override FilterRuleSet
    void doPut(FilterRule const&) override;
    FilterRules doQuery(Uri const&) const override;
    boost::property_tree::ptree doStatistics() const override;
    void doClear() override;

private:
    Rules m_rules;
};

} // namespace adblock

#endif // ADBLOCK_DOMAIN_MATCH_FILTER_RULE_SET_HPP
