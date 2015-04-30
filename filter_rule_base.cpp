#include "filter_rule_base.hpp"

#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"

#include <cassert>

namespace adblock {

void FilterRuleBase::
put(const FilterRulePtr &ruleP)
{
    assert(ruleP);
    const auto &rule = *ruleP;

    if (typeid(rule) == typeid(BasicFilterRule)) {
        m_normal.put(ruleP);
    }
    else if (typeid(rule) == typeid(ExceptionFilterRule)) {
        m_exception.put(ruleP);
    }
    else {
        assert(false && "unknown type of filter rule");
    }
}

bool FilterRuleBase::
query(const Uri &uri, const Context &context) const
{
    assert(uri.is_valid());

    if (m_exception.query(uri, context)) {
        return false;
    }
    else {
        return m_normal.query(uri, context);
    }
}


void FilterRuleBase::FilterRuleGroup::
put(const FilterRulePtr &rule)
{
    assert(rule);

    const auto &pattern = rule->pattern();
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
        m_regex.push_back(rule);
    }
    else {
        assert(false && "unknown type of match pattern");
    }
}

bool FilterRuleBase::FilterRuleGroup::
query(const Uri &uri, const Context &context) const
{
    assert(uri.is_valid());

    std::cout << "URI: " << uri.string() << "\n";
    for (const auto &rule: m_prefix.query(uri)) {
        if (rule->match(uri, context)) return true;
    }
    for (const auto &rule: m_suffix.query(uri)) {
        if (rule->match(uri, context)) return true;
    }
    for (const auto &rule: m_domain.query(uri)) {
        if (rule->match(uri, context)) return true;
    }
    for (const auto &rule: m_substring.query(uri)) {
        std::cout << "SUBSTR RULE: " << *rule << "\n";
        if (rule->match(uri, context)) return true;
    }
    for (const auto &rule: m_regex) {
        if (rule->match(uri, context)) return true;
    }

    return false;
}

} // namespace adblock
