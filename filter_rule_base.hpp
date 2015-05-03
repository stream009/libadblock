#ifndef ADBLOCK_FILTER_RULE_BASE_HPP
#define ADBLOCK_FILTER_RULE_BASE_HPP

#include "type.hpp"
#include "context.hpp"
#include "rule_set/prefix_match_filter_rule_set.hpp"
#include "rule_set/suffix_match_filter_rule_set.hpp"
#include "rule_set/substring_match_filter_rule_set.hpp"
#include "rule_set/domain_match_filter_rule_set.hpp"

#include <memory>

namespace adblock {

class FilterRuleBase
{
private:
    struct FilterRuleGroup
    {
        // query
        bool query(const Uri&, const Context&) const;
        void statistics(std::ostream&) const;

        // modifier
        void put(const FilterRule&);

        PrefixMatchFilterRuleSet m_prefix;
        SuffixMatchFilterRuleSet m_suffix;
        SubstringMatchFilterRuleSet m_substring;
        DomainMatchFilterRuleSet m_domain;
        std::vector<const FilterRule*> m_regex;
    };

public:
    // query
    bool query(const Uri&, const Context&) const;
    void statistics(std::ostream&) const;

    // modifier
    void put(const FilterRule&);

private:
    FilterRuleGroup m_normal;
    FilterRuleGroup m_exception;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_RULE_BASE_HPP
