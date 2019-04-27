#ifndef ADBLOCK_FILTER_RULE_INDEX_HPP
#define ADBLOCK_FILTER_RULE_INDEX_HPP

#include "prefix_match_filter_rule_map.hpp"
#include "suffix_match_filter_rule_map.hpp"
#include "substring_match_filter_rule_map.hpp"
#include "domain_match_filter_rule_map.hpp"

#include "core/json_fwd.hpp"

#include <vector>

namespace adblock {

class Context;
class FilterRule;
class Uri;

class FilterRuleIndex
{
public:
    // query
    FilterRule const*
        query(Uri const&, Context const&,
                          bool genericDisabled = false) const;

    // modifier
    void put(FilterRule const&);
    void clear();

    json::object statistics() const;

private:
    PrefixMatchFilterRuleMap m_prefix;
    SuffixMatchFilterRuleMap m_suffix;
    SubstringMatchFilterRuleMap m_substring;
    DomainMatchFilterRuleMap m_domain;
    std::vector<FilterRule const*> m_regex;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_RULE_INDEX_HPP
