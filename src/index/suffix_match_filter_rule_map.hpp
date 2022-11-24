#ifndef ADBLOCK_SUFFIX_MATCH_FILTER_RULE_MAP_HPP
#define ADBLOCK_SUFFIX_MATCH_FILTER_RULE_MAP_HPP

#include "filter_rule_map.hpp"
#include "namespace.hpp"

#include "core/json.hpp"

#include <iterator>

#include <stream9/radix_tree.hpp>

namespace adblock {

class Uri;

class SuffixMatchFilterRuleMap : public FilterRuleMap
{
    using Base = FilterRuleMap;
public:
    using Base::FilterRules;
    using ReverseStringRange =
        boost::iterator_range<std::reverse_iterator<char const*>>;
    using Map = st9::radix_tree::tree<ReverseStringRange, FilterRule const*>;

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
