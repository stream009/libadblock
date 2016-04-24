#include "element_hide_rule_base.hpp"

#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

namespace adblock {

using ElementHideRules = ElementHideRuleBase::ElementHideRules;

static std::string
joinSelectors(const ElementHideRules &rules, const StringRange &separator)
{
    if (rules.empty()) return "";

    auto it = rules.begin();
    const auto end = rules.end();
    assert(it != end);

    std::string result;
    const auto &back_inserter = std::back_inserter(result);

    boost::copy((*it)->cssSelector(), back_inserter);
    while (++it != end) {
        boost::copy(separator, back_inserter);
        boost::copy((*it)->cssSelector(), back_inserter);
    }

    return result;
}

static void
removeWhiteRules(ElementHideRules &rules, const ElementHideRules &whiteRules)
{
    if (whiteRules.empty() || rules.empty()) return;

    const auto begin = rules.begin();
    auto end = rules.end();
    for (const auto &whiteRule: whiteRules) {
        end = std::remove_if(begin, end,
            [&whiteRule](const ElementHideRule *rule) {
                return rule->cssSelector() == whiteRule->cssSelector();
            }
        );
    }
    rules.erase(end, rules.end());
}


std::string ElementHideRuleBase::
query(const Uri &uri) const
{
    ElementHideRules resultSet;

    if (!genericDisabled(uri)) {
        resultSet = m_blackList;
    }

    const auto &domainedBlackList = m_domainedBlackList.query(uri);
    resultSet.insert(resultSet.end(),
                     domainedBlackList.begin(), domainedBlackList.end());

    const auto &domainedWhiteList = m_domainedWhiteList.query(uri);
    removeWhiteRules(resultSet, domainedWhiteList);

    return resultSet.empty() ? ""
        : joinSelectors(resultSet, ", "_r) + " { display: none !important }";
}

void ElementHideRuleBase::
put(const ElementHideRule &rule)
{
    if (typeid(rule) == typeid(BasicElementHideRule)) {
        if (rule.isDomainRestricted()) {
            m_domainedBlackList.put(rule);
        }
        else {
            m_blackList.push_back(&rule);
        }
    }
    else if (typeid(rule) == typeid(ExceptionElementHideRule)) {
        assert(rule.isDomainRestricted());
        m_domainedWhiteList.put(rule);
    }
    else {
        assert(false && "unknown type of element hide rule");
    }
}

void ElementHideRuleBase::
putGenericDisablerRule(const FilterRule &rule)
{
    m_genericDisabled.put(rule);
}

boost::property_tree::ptree ElementHideRuleBase::
statistics() const
{
    boost::property_tree::ptree result, detail;

    size_t total = 0u;

    auto num = m_blackList.size();
    total += num;
    result.put("Normal element hide rule", num);

    auto stats = m_domainedBlackList.statistics();
    num = stats.get<size_t>("Number of values");
    num += stats.get<size_t>("Exception only rules");
    total += num;
    result.put("Domained black list", num);
    detail.put_child("Domained black list", stats);

    stats = m_domainedWhiteList.statistics();
    num = stats.get<size_t>("Number of values");
    num += stats.get<size_t>("Exception only rules");
    total += num;
    result.put("Domained white list", num);
    detail.put_child("Domained white list", stats);

    result.put("Total", total);
    result.put_child("detail", detail);

    return result;
}

void ElementHideRuleBase::
clear()
{
    m_domainedBlackList.clear();
    m_domainedWhiteList.clear();
    m_blackList.clear();
}

bool ElementHideRuleBase::
genericDisabled(const Uri &uri) const
{
    const auto* const rule = m_genericDisabled.query(uri);

    return rule != nullptr;
}

} // namespace adblock
