#ifndef ADBLOCK_DOMAIN_MATCH_FILTER_RULE_MAP_HPP
#define ADBLOCK_DOMAIN_MATCH_FILTER_RULE_MAP_HPP

#include "filter_rule_map.hpp"
#include "core/type.hpp"

#include <boost/property_tree/ptree_fwd.hpp>

#include <radix_tree/radix_tree.hpp>

namespace adblock {

class DomainMatchFilterRuleMap : public FilterRuleMap
{
    using Base = FilterRuleMap;
public:
    using Base::FilterRules;
    using Map = radix_tree::RadixTree<StringRange, FilterRule const*>;

private:
    // @override FilterRuleMap
    void doPut(FilterRule const&) override;
    FilterRules doQuery(Uri const&) const override;
    boost::property_tree::ptree doStatistics() const override;
    void doClear() override;

private:
    Map m_rules;
};

} // namespace adblock

#endif // ADBLOCK_DOMAIN_MATCH_FILTER_RULE_MAP_HPP
