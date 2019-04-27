#ifndef ADBLOCK_FILTER_RULE_BASE_HPP
#define ADBLOCK_FILTER_RULE_BASE_HPP

#include "json_fwd.hpp"

#include "index/filter_rule_index.hpp"

namespace adblock {

class Context;
class StringRange;
class Uri;

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

    json::object statistics() const;

    // modifier
    void put(FilterRule const&);

    void clear();

private:
    FilterRuleIndex m_normal;
    FilterRuleIndex m_exception;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_RULE_BASE_HPP
