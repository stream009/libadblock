#ifndef ADBLOCK_FILTER_RULE_SET_HPP
#define ADBLOCK_FILTER_RULE_SET_HPP

#include "type.hpp"
#include "rule/filter_rule.hpp"

#include <vector>

#include <boost/property_tree/ptree_fwd.hpp>

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
    void clear() { doClear(); }

    // query
    FilterRules query(const Uri &uri) const { return doQuery(uri); }
    boost::property_tree::ptree statistics() const { return doStatistics(); }

private:
    virtual void doPut(const FilterRule&) = 0;
    virtual FilterRules doQuery(const Uri&) const = 0;
    virtual boost::property_tree::ptree doStatistics() const = 0;
    virtual void doClear() = 0;
};

} // namespace adblock

#endif // ADBLOCK_FILTER_RULE_SET_HPP
