#ifndef ADBLOCK_FILTER_RULE_SET_HPP
#define ADBLOCK_FILTER_RULE_SET_HPP

#include "type.hpp"
#include "rule/filter_rule.hpp"

#include <iosfwd>
#include <vector>

namespace adblock {

class FilterRuleSet
{
public:
    using FilterRules = std::vector<const FilterRule*>;
public:
    // constructor / destructor
    virtual ~FilterRuleSet() = default;

    // modifier
    void put(const FilterRule &rule) { doPut(rule); }

    // query
    FilterRules query(const Uri &uri) const { return doQuery(uri); }
    void statistics(std::ostream &os) const { return doStatistics(os); }

private:
    virtual void doPut(const FilterRule&) = 0;
    virtual FilterRules doQuery(const Uri&) const = 0;
    virtual void doStatistics(std::ostream&) const = 0;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_RULE_SET_HPP
