#ifndef ADBLOCK_FILTER_RULE_MAP_HPP
#define ADBLOCK_FILTER_RULE_MAP_HPP

#include "core/json.hpp"
#include "rule/filter_rule.hpp"

#include <vector>

namespace adblock {

class FilterRuleMap
{
public:
    using FilterRules = std::vector<FilterRule const*>;
public:
    // constructor / destructor
    virtual ~FilterRuleMap() = default;

    // modifier
    void put(FilterRule const& rule) { doPut(rule); }
    void clear() { doClear(); }

    // query
    FilterRules query(Uri const& uri) const { return doQuery(uri); }
    json::object statistics() const { return doStatistics(); }

private:
    virtual void doPut(FilterRule const&) = 0;
    virtual FilterRules doQuery(Uri const&) const = 0;
    virtual json::object doStatistics() const = 0;
    virtual void doClear() = 0;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_RULE_MAP_HPP
