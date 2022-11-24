#ifndef ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_MAP_HPP
#define ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_MAP_HPP

#include "namespace.hpp"

#include "core/json.hpp"
#include "rule/element_hide_rule.hpp"

#include <iterator>
#include <vector>

#include <stream9/radix_tree.hpp>

namespace adblock {

class Uri;

class DomainedElementHideRuleMap
{
public:
    using ElementHideRules = std::vector<ElementHideRule const*>;
    using ReverseStringRange =
        boost::iterator_range<std::reverse_iterator<char const*>>;
    using Map = st9::radix_tree::tree<ReverseStringRange, ElementHideRule const*>;
    using ExceptionOnlyRules = std::vector<ElementHideRule const*>;

public:
    // query
    ElementHideRules query(Uri const&) const;
    json::object statistics() const;

    // modifier
    void put(ElementHideRule const&);
    void clear();

private:
    Map m_normal;
    ExceptionOnlyRules m_exception;
};

} // namespace adblock

#endif // ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_MAP_HPP
