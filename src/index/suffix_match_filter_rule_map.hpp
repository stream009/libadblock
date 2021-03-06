#ifndef ADBLOCK_SUFFIX_MATCH_FILTER_RULE_MAP_HPP
#define ADBLOCK_SUFFIX_MATCH_FILTER_RULE_MAP_HPP

#include "filter_rule_map.hpp"

#include "core/json_fwd.hpp"

#include <iterator>

#include <radix_tree/radix_tree.hpp>

namespace adblock {

class Uri;

class SuffixMatchFilterRuleMap : public FilterRuleMap
{
    using Base = FilterRuleMap;
public:
    using Base::FilterRules;
    using ReverseStringRange =
        boost::iterator_range<std::reverse_iterator<char const*>>;
    using Map = radix_tree::tree<ReverseStringRange, FilterRule const*>;

private:
    void doPut(FilterRule const&) override;
    FilterRules doQuery(Uri const&) const override;
    json::object doStatistics() const override;
    void doClear() override;

private:
    Map m_rules;
};

} // namespace adblock

#endif // ADBLOCK_SUFFIX_MATCH_FILTER_RULE_MAP_HPP
