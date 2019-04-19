#ifndef ADBLOCK_SUFFIX_MATCH_FILTER_RULE_MAP_HPP
#define ADBLOCK_SUFFIX_MATCH_FILTER_RULE_MAP_HPP

#include "filter_rule_map.hpp"
#include "core/type.hpp"

#include <iterator>

#include <boost/property_tree/ptree_fwd.hpp>

#include <radix_tree/radix_tree.hpp>

namespace adblock {

class SuffixMatchFilterRuleMap : public FilterRuleMap
{
    using Base = FilterRuleMap;
public:
    using Base::FilterRules;
    using ReverseStringRange =
        boost::iterator_range<std::reverse_iterator<char const*>>;
    using Map = radix_tree::RadixTree<ReverseStringRange, FilterRule const*>;

private:
    void doPut(FilterRule const&) override;
    FilterRules doQuery(Uri const&) const override;
    boost::property_tree::ptree doStatistics() const override;
    void doClear() override;

private:
    Map m_rules;
};

} // namespace adblock

#endif // ADBLOCK_SUFFIX_MATCH_FILTER_RULE_MAP_HPP
