#ifndef ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_MAP_HPP
#define ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_MAP_HPP

#include "type.hpp"
#include "rule/element_hide_rule.hpp"

#include <memory>
#include <vector>

#include <boost/container/flat_set.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

#include <radix_tree/radix_tree.hpp>

namespace adblock {

class DomainedElementHideRuleMap
{
public:
    using ElementHideRules = std::vector<ElementHideRule const*>;
    using ReverseStringRange =
        boost::iterator_range<std::reverse_iterator<char const*>>;
    using Map = radix_tree::RadixTree<ReverseStringRange, ElementHideRule const*>;
    using ExceptionOnlyRules = boost::container::flat_set<ElementHideRule const*>;

public:
    // query
    ElementHideRules query(Uri const&) const;
    boost::property_tree::ptree statistics() const;

    // modifier
    void put(ElementHideRule const&);
    void clear();

private:
    Map m_normal;
    ExceptionOnlyRules m_exception;
};

} // namespace adblock

#endif // ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_MAP_HPP
