#ifndef ADBLOCK_DOMAIN_MATCH_FILTER_RULE_MAP_HPP
#define ADBLOCK_DOMAIN_MATCH_FILTER_RULE_MAP_HPP

#include "filter_rule_map.hpp"

#include "core/json_fwd.hpp"
#include "core/string_range.hpp"

#include <radix_tree/radix_tree.hpp>

namespace adblock {

class DomainMatchFilterRuleMap : public FilterRuleMap
{
    using Base = FilterRuleMap;
public:
    using Base::FilterRules;
    using Map = radix_tree::tree<StringRange, FilterRule const*>;

private:
    // @override FilterRuleMap
    void doPut(FilterRule const&) override;
    FilterRules doQuery(Uri const&) const override;
    json::object doStatistics() const override;
    void doClear() override;

private:
    Map m_rules;
};

} // namespace adblock

#endif // ADBLOCK_DOMAIN_MATCH_FILTER_RULE_MAP_HPP
