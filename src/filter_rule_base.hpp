#ifndef ADBLOCK_FILTER_RULE_BASE_HPP
#define ADBLOCK_FILTER_RULE_BASE_HPP

#include "type.hpp"
#include "filter_rule_group.hpp"

#include <boost/property_tree/ptree_fwd.hpp>

namespace adblock {

class Context;

class FilterRuleBase
{
public:
    // query
    std::pair<bool, FilterRule const*>
        query(Uri const&, Context const&) const;

    FilterRule const* getFrameBlockDisabler(
                         Uri const&, StringRange siteKey) const;
    FilterRule const* getGenericBlockDisabler(
                         Uri const&, StringRange siteKey) const;

    FilterRule const* getFrameHideDisabler(
                          Uri const&, StringRange siteKey) const;
    FilterRule const* getGenericHideDisabler(
                          Uri const&, StringRange siteKey) const;

    boost::property_tree::ptree statistics() const;

    // modifier
    void put(FilterRule const&);

    void clear();

private:
    FilterRuleGroup m_normal;
    FilterRuleGroup m_exception;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_RULE_BASE_HPP
