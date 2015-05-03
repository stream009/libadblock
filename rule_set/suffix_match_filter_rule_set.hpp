#ifndef ADBLOCK_SUFFIX_MATCH_FILTER_RULE_SET_HPP
#define ADBLOCK_SUFFIX_MATCH_FILTER_RULE_SET_HPP

#include "filter_rule_set.hpp"
#include "type.hpp"
#include "trie/trie.hpp"

#include <iosfwd>
#include <iterator>

namespace adblock {

class SuffixMatchFilterRuleSet : public FilterRuleSet
{
    using Base = FilterRuleSet;
public:
    using Base::FilterRules;
    using ReverseStringRange =
        boost::iterator_range<std::reverse_iterator<const char*>>;
    using Rules = trie::Trie<ReverseStringRange, const FilterRule*>;

private:
    void doPut(const FilterRule&) override;
    FilterRules doQuery(const Uri&) const override;
    void doStatistics(std::ostream&) const override;

private:
    Rules m_rules;
};

} // namespace adblock

#endif // ADBLOCK_SUFFIX_MATCH_FILTER_RULE_SET_HPP
