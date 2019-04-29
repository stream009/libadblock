#include "element_hide_rule_base.hpp"

#include "filter_rule_base.hpp"
#include "json.hpp"
#include "string_range.hpp"
#include "uri.hpp"

#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "rule/extended_element_hide_rule.hpp"

namespace adblock {

using ElementHideRules = ElementHideRuleBase::ElementHideRules;

static void
removeWhiteRules(ElementHideRules &rules, ElementHideRules const& whiteRules)
{
    if (whiteRules.empty() || rules.empty()) return;

    auto const begin = rules.begin();
    auto end = rules.end();
    for (auto const& whiteRule: whiteRules) {
        end = std::remove_if(begin, end,
            [&](auto* rule) {
                return rule->cssSelector() == whiteRule->cssSelector();
            }
        );
    }
    rules.erase(end, rules.end());
}

//
// ELementHideRuleBase
//
ElementHideRuleBase::
ElementHideRuleBase(FilterRuleBase const& filterRuleBase)
    : m_filterRuleBase { filterRuleBase }
{}

ElementHideRuleBase::~ElementHideRuleBase() = default;

ElementHideRules ElementHideRuleBase::
lookupRules(Uri const& uri, StringRange const siteKey/*= {}*/) const
{
    ElementHideRules result;

    if (isWhiteListed(uri, siteKey)) return result;

    auto const specificOnly = isDomainSpecificOnly(uri, siteKey);

    if (!specificOnly) {
        result = m_genericBlackList;
    }

    auto const& domainedBlackList = m_domainedBlackList.query(uri);
    result.insert(result.end(),
                  domainedBlackList.begin(), domainedBlackList.end());

    auto const& domainedWhiteList = m_domainedWhiteList.query(uri);
    removeWhiteRules(result, domainedWhiteList);

    if (!specificOnly) {
        removeWhiteRules(result, m_genericWhiteList);
    }

    return result;
}

ElementHideRules ElementHideRuleBase::
lookupExtendedRules(Uri const& uri, StringRange const siteKey/*= {}*/) const
{
    ElementHideRules result;

    if (isWhiteListed(uri, siteKey)) return result;

    auto const specificOnly = isDomainSpecificOnly(uri, siteKey);

    if (!specificOnly) {
        result = m_genericExtendedBlackList;
    }

    auto const& domainedBlackList = m_domainedExtendedBlackList.query(uri);
    result.insert(result.end(),
                  domainedBlackList.begin(), domainedBlackList.end());

    auto const& domainedWhiteList = m_domainedWhiteList.query(uri);
    removeWhiteRules(result, domainedWhiteList);

    if (!specificOnly) {
        removeWhiteRules(result, m_genericWhiteList);
    }

    return result;
}

void ElementHideRuleBase::
put(ElementHideRule const& rule)
{
    if (typeid(rule) == typeid(BasicElementHideRule)) {
        if (rule.isDomainRestricted()) {
            m_domainedBlackList.put(rule);
        }
        else {
            m_genericBlackList.push_back(&rule);
        }
    }
    else if (typeid(rule) == typeid(ExceptionElementHideRule)) {
        if (rule.isDomainRestricted()) {
            m_domainedWhiteList.put(rule);
        }
        else {
            m_genericWhiteList.push_back(&rule);
        }
    }
    else if (typeid(rule) == typeid(ExtendedElementHideRule)) {
        if (rule.isDomainRestricted()) {
            m_domainedExtendedBlackList.put(rule);
        }
        else {
            m_genericExtendedBlackList.push_back(&rule);
        }
    }
    else {
        assert(false && "unknown type of element hide rule");
    }
}

json::object ElementHideRuleBase::
statistics() const
{
    json::object result, detail;

    double total = 0;

    auto num = static_cast<double>(m_genericBlackList.size());
    total += num;
    result["Generic black list"] = num;

    num = static_cast<double>(m_genericWhiteList.size());
    total += num;
    result["Generic white list"] = num;

    auto stats = m_domainedBlackList.statistics();
    num = to_number(stats["Number of values"]);
    num += to_number(stats["Exception only rules"]);
    total += num;
    result["Domained black list"] = num;
    detail["Domained black list"] = stats;

    stats = m_domainedWhiteList.statistics();
    num = to_number(stats["Number of values"]);
    num += to_number(stats["Exception only rules"]);
    total += num;
    result["Domained white list"] = num;
    detail["Domained white list"] = stats;

    num = static_cast<double>(m_genericExtendedBlackList.size());
    total += num;
    result["Extended element hide rule"] = num;

    stats = m_domainedExtendedBlackList.statistics();
    num = to_number(stats["Number of values"]);
    num += to_number(stats["Exception only rules"]);
    total += num;
    result["Domained extended black list"] = num;
    detail["Domained extended black list"] = stats;

    result["Total"] = total;
    result["detail"] = detail;

    return result;
}

void ElementHideRuleBase::
clear()
{
    m_domainedBlackList.clear();
    m_domainedWhiteList.clear();

    m_genericBlackList.clear();
    m_genericWhiteList.clear();

    m_domainedExtendedBlackList.clear();
    m_genericExtendedBlackList.clear();
}

bool ElementHideRuleBase::
isWhiteListed(Uri const& uri, StringRange const siteKey) const
{
    return m_filterRuleBase.getFrameHideDisabler(uri, siteKey) != nullptr;
}

bool ElementHideRuleBase::
isDomainSpecificOnly(Uri const& uri, StringRange const siteKey) const
{
    return m_filterRuleBase.getGenericHideDisabler(uri, siteKey) != nullptr;
}

} // namespace adblock
