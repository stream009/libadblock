#ifndef ADBLOCK_PREFIX_MATCH_FILTER_RULE_MAP_HPP
#define ADBLOCK_PREFIX_MATCH_FILTER_RULE_MAP_HPP

#include "filter_rule_map.hpp"
#include "core/type.hpp"

#include <boost/property_tree/ptree_fwd.hpp>

#include <radix_tree/radix_tree.hpp>

namespace adblock {

class PrefixMatchFilterRuleMap : public FilterRuleMap
{
    using Base = FilterRuleMap;
public:
    using Base::FilterRules;
    using Map = radix_tree::RadixTree<StringRange, FilterRule const*>;

private:
    void doPut(FilterRule const&) override;
    FilterRules doQuery(Uri const&) const override;
    boost::property_tree::ptree doStatistics() const override;
    void doClear() override;

private:
    Map m_rules;
};

} // namespace adblock

#endif // ADBLOCK_PREFIX_MATCH_FILTER_RULE_MAP_HPP
