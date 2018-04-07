#ifndef ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_SET_HPP
#define ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_SET_HPP

#include "config.hpp"
#include "type.hpp"
#include "rule/element_hide_rule.hpp"

#include <memory>
#include <vector>

#include <boost/container/flat_set.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

namespace adblock {

class DomainedElementHideRuleSet
{
public:
    using ElementHideRules = std::vector<const ElementHideRule*>;
    using ReverseStringRange =
        boost::iterator_range<std::reverse_iterator<const char*>>;
    using Rules = rule_set::Rules<ReverseStringRange, const ElementHideRule*>;
    using ExceptionOnlyRules = boost::container::flat_set<const ElementHideRule*>;

public:
    // query
    ElementHideRules query(const Uri&) const;
    boost::property_tree::ptree statistics() const;

    // modifier
    void put(const ElementHideRule&);
    void clear();

private:
    Rules m_normal;
    ExceptionOnlyRules m_exception;
};

} // namespace adblock

#endif // ADBLOCK_DOMAINED_ELEMENT_HIDE_RULE_SET_HPP
