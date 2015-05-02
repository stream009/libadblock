#ifndef ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_SET_HPP
#define ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_SET_HPP

#include "type.hpp"
#include "rule/element_hide_rule.hpp"
#include "trie/trie.hpp"

#include <iosfwd>
#include <memory>
#include <vector>

#include <boost/container/flat_set.hpp>

namespace adblock {

class DomainedElementHideRuleSet
{
public:
    using ElementHideRulePtr = std::shared_ptr<ElementHideRule>;
    using ElementHideRules = std::vector<ElementHideRulePtr>;
    using ReverseStringRange =
        boost::iterator_range<std::reverse_iterator<const char*>>;
    using Rules = trie::Trie<ReverseStringRange, ElementHideRulePtr>;
    using ExceptionOnlyRules = boost::container::flat_set<ElementHideRulePtr>;

public:
    // query
    ElementHideRules query(const Uri&) const;
    void statistics(std::ostream&) const;

    // modifier
    void put(const ElementHideRulePtr&);

private:
    Rules m_normal;
    ExceptionOnlyRules m_exception;
};

} // namespace adblock

#endif // ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_SET_HPP
