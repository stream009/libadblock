#ifndef ADBLOCK_FILTER_RULE_INDEX_HPP
#define ADBLOCK_FILTER_RULE_INDEX_HPP

#include "core/type.hpp"
#include "prefix_match_filter_rule_map.hpp"
#include "suffix_match_filter_rule_map.hpp"
#include "substring_match_filter_rule_map.hpp"
#include "domain_match_filter_rule_map.hpp"

#include <vector>

#include <boost/property_tree/ptree_fwd.hpp>

namespace adblock {

class Context;
class FilterRule;

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

    boost::property_tree::ptree statistics() const;

private:
    PrefixMatchFilterRuleMap m_prefix;
    SuffixMatchFilterRuleMap m_suffix;
    SubstringMatchFilterRuleMap m_substring;
    DomainMatchFilterRuleMap m_domain;
    std::vector<FilterRule const*> m_regex;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_RULE_INDEX_HPP
