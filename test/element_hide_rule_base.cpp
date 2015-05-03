#include "type.hpp"
#include "element_hide_rule_base.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"

#include <memory>

#include <boost/optional.hpp>

#include <gtest/gtest.h>

using namespace adblock;

using Domains = ElementHideRule::Domains;

template<typename T>
static std::shared_ptr<ElementHideRule>
make_rule(const StringRange &cssSelector,
          const boost::optional<Domains> &domains)
{
    return std::make_shared<T>(cssSelector, domains);
}

TEST(ElementHideRuleBase, Elementary)
{
    const auto &rule =
                make_rule<BasicElementHideRule>("div"_r, boost::none);
    assert(rule);
    ElementHideRuleBase ruleBase;
    ruleBase.put(*rule);

    const auto &result = ruleBase.query("http://www.adblock.org"_u);
    EXPECT_EQ("div { display: none !important }", result);
}

TEST(ElementHideRuleBase, Domained)
{
    const auto &rule1 =
                make_rule<BasicElementHideRule>("div"_r, boost::none);
    const auto &rule2 = make_rule<BasicElementHideRule>(
                                    "table"_r, Domains { "adblock.org"_r });
    ElementHideRuleBase ruleBase;
    assert(rule1);
    ruleBase.put(*rule1);
    assert(rule2);
    ruleBase.put(*rule2);

    const auto &result = ruleBase.query("http://www.adblock.org"_u);
    EXPECT_EQ("table, div { display: none !important }", result);
}

TEST(ElementHideRuleBase, ExcludedByExceptionRule)
{
    const auto &rule1 =
                make_rule<BasicElementHideRule>("div"_r, boost::none);
    const auto &rule2 = make_rule<ExceptionElementHideRule>(
                                    "div"_r, Domains { "adblock.org"_r });
    ElementHideRuleBase ruleBase;
    assert(rule1);
    ruleBase.put(*rule1);
    assert(rule2);
    ruleBase.put(*rule2);

    const auto &result = ruleBase.query("http://www.adblock.org"_u);
    EXPECT_EQ("", result);
}

TEST(ElementHideRuleBase, DomainMatchWithExceptionRuleButSelectorIsnTSame)
{
    const auto &rule1 =
                make_rule<BasicElementHideRule>("div"_r, boost::none);
    const auto &rule2 = make_rule<ExceptionElementHideRule>(
                                    "table"_r, Domains { "adblock.org"_r });
    ElementHideRuleBase ruleBase;
    assert(rule1);
    ruleBase.put(*rule1);
    assert(rule2);
    ruleBase.put(*rule2);

    const auto &result = ruleBase.query("http://www.adblock.org"_u);
    EXPECT_EQ("div { display: none !important }", result);
}
