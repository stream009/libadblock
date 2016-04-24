#include "filter_rule_base.hpp"

#include "context.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"

#include <cassert>
#include <utility>

#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>

namespace adblock {

void FilterRuleBase::
put(const FilterRule &rule)
{
    if (typeid(rule) == typeid(BasicFilterRule)) {
        m_normal.put(rule);
    }
    else if (typeid(rule) == typeid(ExceptionFilterRule)) {
        m_exception.put(rule);
    }
    else {
        assert(false && "unknown type of filter rule");
    }
}

void FilterRuleBase::
putGenericDisablerRule(const FilterRule &rule)
{
    m_genericDisabled.put(rule);
}

void FilterRuleBase::
clear()
{
    m_normal.clear();
    m_exception.clear();
}

std::pair<bool, const FilterRule*> FilterRuleBase::
query(const Uri &uri, const Context &context) const
{
    assert(uri.is_valid());

    if (const auto *rule = m_exception.query(uri, &context)) {
        return std::make_pair(false, rule);
    }
    else {
        rule = m_normal.query(uri, &context, genericDisabled(context));
        return std::make_pair(rule != nullptr, rule);
    }
}

bool FilterRuleBase::
genericDisabled(const Context &context) const
{
    const auto &uri = context.origin();
    const auto* const rule = m_genericDisabled.query(uri, &context);

    return rule != nullptr;
}

boost::property_tree::ptree FilterRuleBase::
statistics() const
{
    boost::property_tree::ptree result, detail;

    size_t total = 0u;

    auto stats = m_normal.statistics();
    auto num = stats.get<size_t>("Total");
    total += num;
    result.put("Basic match pattern", num);
    detail.put_child("Basic match pattern", stats);

    stats = m_exception.statistics();
    num = stats.get<size_t>("Total");
    total += num;
    result.put("Exception match pattern", num);
    detail.put_child("Exception match pattern", stats);

    result.put("Total", total);

    result.put_child("detail", detail);

    return result;
}

} // namespace adblock
