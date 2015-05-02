#ifndef ADBLOCK_FILTER_RULE_SET_HPP
#define ADBLOCK_FILTER_RULE_SET_HPP

#include "type.hpp"
#include "rule/filter_rule.hpp"

#include <memory>
#include <ostream>
#include <vector>

namespace adblock {

class FilterRuleSet
{
public:
    using FilterRulePtr = std::shared_ptr<FilterRule>;
    using FilterRules = std::vector<FilterRulePtr>;
public:
    // constructor / destructor
    virtual ~FilterRuleSet() = default;

    // modifier
    void put(const FilterRulePtr &rule) { doPut(rule); }

    // query
    FilterRules query(const Uri &uri) const { return doQuery(uri); }
    void statistics(std::ostream &os) const { return doStatistics(os); }

private:
    virtual void doPut(const FilterRulePtr&) = 0;
    virtual FilterRules doQuery(const Uri&) const = 0;
    virtual void doStatistics(std::ostream&) const = 0;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_RULE_SET_HPP
