#include "element_hide_rule_base.hpp"

#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"

#include <sstream>

#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>

namespace adblock {

using ElementHideRules = ElementHideRuleBase::ElementHideRules;
using ElementHideRulePtr = ElementHideRuleBase::ElementHideRulePtr;

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
            [&whiteRule](const ElementHideRulePtr &rule) {
                return rule->cssSelector() == whiteRule->cssSelector();
            }
        );
    }
    rules.erase(end, rules.end());
}


std::string ElementHideRuleBase::
query(const Uri &uri) const
{
    const auto &domainedBlackList = m_domainedBlackList.query(uri);
    const auto &domainedWhiteList = m_domainedWhiteList.query(uri);

    ElementHideRules resultSet;
    boost::copy(domainedBlackList, std::back_inserter(resultSet));
    boost::copy(m_blackList, std::back_inserter(resultSet));

    removeWhiteRules(resultSet, domainedWhiteList);

    return resultSet.empty() ? ""
        : joinSelectors(resultSet, ", "_r) + " { display: none !important }";
}

void ElementHideRuleBase::
put(const ElementHideRulePtr &ruleP)
{
    const auto &rule = *ruleP;

    if (typeid(rule) == typeid(BasicElementHideRule)) {
        if (ruleP->isDomainRestricted()) {
            m_domainedBlackList.put(ruleP);
        }
        else {
            m_blackList.push_back(ruleP);
        }
    }
    else if (typeid(rule) == typeid(ExceptionElementHideRule)) {
        assert(ruleP->isDomainRestricted());
        m_domainedWhiteList.put(ruleP);
    }
    else {
        assert(false && "unknown type of element hide rule");
    }
}

void ElementHideRuleBase::
statistics(std::ostream &os) const
{
    os << "Normal Element Hide Rule\n";
    os << boost::format { "%20s: %6s\n" } % "Total" % m_blackList.size();
    os << "\n";

    os << "Domained Black List\n";
    m_domainedBlackList.statistics(os);
    os << "\n";

    os << "Domained White List\n";
    m_domainedWhiteList.statistics(os);
    os << "\n";
}

} // namespace adblock
