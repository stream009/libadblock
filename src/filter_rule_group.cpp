#include "filter_rule_group.hpp"

#include "context.hpp"
#include "rule/filter_rule.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"

#include <cassert>

#include <boost/property_tree/ptree.hpp>

namespace adblock {

void FilterRuleGroup::
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

void FilterRuleGroup::
clear()
{
    m_prefix.clear();
    m_suffix.clear();
    m_substring.clear();
    m_domain.clear();
    m_regex.clear();
}

const FilterRule *FilterRuleGroup::
query(Uri const& uri, Context const& context,
                      bool const specificOnly/*= false*/) const
{
    for (const auto *rule: m_prefix.query(uri)) {
        if (rule->match(uri, context, specificOnly)) return rule;
    }
    for (const auto *rule: m_suffix.query(uri)) {
        if (rule->match(uri, context, specificOnly)) return rule;
    }
    for (const auto *rule: m_domain.query(uri)) {
        if (rule->match(uri, context, specificOnly)) return rule;
    }
    for (const auto *rule: m_substring.query(uri)) {
        if (rule->match(uri, context, specificOnly)) return rule;
    }
    for (const auto *rule: m_regex) {
        if (rule->match(uri, context, specificOnly)) return rule;
    }

    return nullptr;
}

boost::property_tree::ptree FilterRuleGroup::
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
