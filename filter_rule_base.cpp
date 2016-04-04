#include "filter_rule_base.hpp"

#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"
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
clear()
{
    m_normal.clear();
    m_exception.clear();
}

std::pair<bool, const FilterRule*> FilterRuleBase::
query(const Uri &uri, const Context &context) const
{
    assert(uri.is_valid());

    if (const auto *rule = m_exception.query(uri, context)) {
        return std::make_pair(false, rule);
    }
    else {
        rule = m_normal.query(uri, context);
        return std::make_pair(rule != nullptr, rule);
    }
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


void FilterRuleBase::FilterRuleGroup::
put(const FilterRule &rule)
{
    const auto &pattern = rule.pattern();
    if (const auto *patternP =
                dynamic_cast<const BasicMatchPattern*>(&pattern))
    {
        if (patternP->isBeginMatch()) {
            m_prefix.put(rule);
        }
        else if (patternP->isEndMatch()) {
            m_suffix.put(rule);
        }
        else {
            m_substring.put(rule);
        }
    }
    else if (typeid(pattern) == typeid(DomainMatchPattern)) {
        m_domain.put(rule);
    }
    else if (typeid(pattern) == typeid(RegexPattern)) {
        m_regex.push_back(&rule);
    }
    else {
        assert(false && "unknown type of match pattern");
    }
}

void FilterRuleBase::FilterRuleGroup::
clear()
{
    m_prefix.clear();
    m_suffix.clear();
    m_substring.clear();
    m_domain.clear();
    m_regex.clear();
}

const FilterRule *FilterRuleBase::FilterRuleGroup::
query(const Uri &uri, const Context &context) const
{
    assert(uri.is_valid());

    for (const auto *rule: m_prefix.query(uri)) {
        if (rule->match(uri, context)) return rule;
    }
    for (const auto *rule: m_suffix.query(uri)) {
        if (rule->match(uri, context)) return rule;
    }
    for (const auto *rule: m_domain.query(uri)) {
        if (rule->match(uri, context)) return rule;
    }
    for (const auto *rule: m_substring.query(uri)) {
        if (rule->match(uri, context)) return rule;
    }
    for (const auto *rule: m_regex) {
        if (rule->match(uri, context)) return rule;
    }

    return nullptr;
}

boost::property_tree::ptree FilterRuleBase::FilterRuleGroup::
statistics() const
{
    boost::property_tree::ptree result, detail;

    size_t total = 0u;

    auto child = m_prefix.statistics();
    auto num = child.get<size_t>("Number of values");
    total += num;
    result.put("Prefix match pattern", num);
    detail.put_child("Prefix match pattern", child);

    child = m_suffix.statistics();
    num = child.get<size_t>("Number of values");
    total += num;
    result.put("Suffix match pattern", num);
    detail.put_child("Suffix match pattern", child);

    child = m_substring.statistics();
    num = child.get<size_t>("Number of values");
    total += num;
    result.put("Substring match pattern", num);
    detail.put_child("Substring match pattern", child);

    child = m_domain.statistics();
    num = child.get<size_t>("Number of values");
    total += num;
    result.put("Domain match pattern", num);
    detail.put_child("Domain match pattern", child);

    num = m_regex.size();
    total += num;
    result.put("Regex pattern", num);

    result.put("Total", total);

    result.put_child("detail", detail);

    return result;
}

} // namespace adblock
