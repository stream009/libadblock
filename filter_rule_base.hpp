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
    std::pair<bool, const FilterRule*>
        query(const Uri&, const Context&) const;

    FilterRule const* getFrameBlockDisabler(
                         Uri const&, StringRange const siteKey) const;
    FilterRule const* getGenericBlockDisabler(
                         Uri const&, StringRange const siteKey) const;

    FilterRule const* getFrameHideDisabler(
                          Uri const&, StringRange const siteKey) const;
    FilterRule const* getGenericHideDisabler(
                          Uri const&, StringRange const siteKey) const;

    boost::property_tree::ptree statistics() const;

    // modifier
    void put(const FilterRule&);

    void putGenericDisablerRule(const FilterRule&);

    void clear();

private:
    bool genericDisabled(const Context&) const;

private:
    FilterRuleGroup m_normal;
    FilterRuleGroup m_exception;
    FilterRuleGroup m_genericDisabled;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_RULE_BASE_HPP
