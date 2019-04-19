#ifndef ADBLOCK_FILTER_RULE_GROUP_HPP
#define ADBLOCK_FILTER_RULE_GROUP_HPP

#include "type.hpp"
#include "rule_set/prefix_match_filter_rule_set.hpp"
#include "rule_set/suffix_match_filter_rule_set.hpp"
#include "rule_set/substring_match_filter_rule_set.hpp"
#include "rule_set/domain_match_filter_rule_set.hpp"

#include <vector>

#include <boost/property_tree/ptree_fwd.hpp>

namespace adblock {

class Context;
class FilterRule;

class FilterRuleGroup
{
public:
    // query
    FilterRule const* query(Uri const&, Context const&,
                            bool genericDisabled = false) const;

    boost::property_tree::ptree statistics() const;

    // modifier
    void put(FilterRule const&);
    void clear();

private:
    PrefixMatchFilterRuleSet m_prefix;
    SuffixMatchFilterRuleSet m_suffix;
    SubstringMatchFilterRuleSet m_substring;
    DomainMatchFilterRuleSet m_domain;
    std::vector<FilterRule const*> m_regex;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_RULE_GROUP_HPP
