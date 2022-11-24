#ifndef ADBLOCK_PREFIX_MATCH_FILTER_RULE_MAP_HPP
#define ADBLOCK_PREFIX_MATCH_FILTER_RULE_MAP_HPP

#include "filter_rule_map.hpp"
#include "namespace.hpp"

#include "core/json.hpp"
#include "core/string_range.hpp"

#include <stream9/radix_tree.hpp>

namespace adblock {

class Uri;

class PrefixMatchFilterRuleMap : public FilterRuleMap
{
    using Base = FilterRuleMap;
public:
    using Base::FilterRules;
    using Map = st9::radix_tree::tree<StringRange, FilterRule const*>;

private:
    void doPut(FilterRule const&) override;
    FilterRules doQuery(Uri const&) const override;
    json::object doStatistics() const override;
    void doClear() override;

private:
    Map m_rules;
};

} // namespace adblock

#endif // ADBLOCK_PREFIX_MATCH_FILTER_RULE_MAP_HPP
