#include "element_hide_rule_base.hpp"

#include "filter_rule_base.hpp"
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

    std::string result;
    const auto &back_inserter = std::back_inserter(result);

    static const auto unit = 200;
    auto const len = rules.size();

    for (auto i = 0ul; i < len; i += unit) {
        boost::copy(rules.at(i)->cssSelector(), back_inserter);

        auto const to = std::min(len, i + unit);
        for (auto j = i + 1; j < to; ++j) {
            boost::copy(separator, back_inserter);
            boost::copy(rules.at(j)->cssSelector(), back_inserter);
        }
        result.append(" { display: none !important } ");
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


ElementHideRuleBase::
ElementHideRuleBase(FilterRuleBase const& filterRuleBase)
    : m_filterRuleBase { filterRuleBase }
{}

std::string ElementHideRuleBase::
query(Uri const& uri, StringRange const& siteKey/*= {}*/) const
{
    ElementHideRules resultSet;

    auto* const disabler = m_filterRuleBase.getFrameHideDisabler(uri, siteKey);
    if (disabler != nullptr) return {};

    auto* const genericDisabler
                       = m_filterRuleBase.getGenericHideDisabler(uri, siteKey);

    if (genericDisabler == nullptr) {
        resultSet = m_genericBlackList;
    }

    const auto &domainedBlackList = m_domainedBlackList.query(uri);
    resultSet.insert(resultSet.end(),
                     domainedBlackList.begin(), domainedBlackList.end());

    const auto &domainedWhiteList = m_domainedWhiteList.query(uri);
    removeWhiteRules(resultSet, domainedWhiteList);

    return joinSelectors(resultSet, ", "_r);
}

void ElementHideRuleBase::
put(const ElementHideRule &rule)
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
        assert(rule.isDomainRestricted());
        m_domainedWhiteList.put(rule);
    }
    else {
        assert(false && "unknown type of element hide rule");
    }
}

boost::property_tree::ptree ElementHideRuleBase::
statistics() const
{
    boost::property_tree::ptree result, detail;

    size_t total = 0u;

    auto num = m_genericBlackList.size();
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
    m_genericBlackList.clear();
}

} // namespace adblock
