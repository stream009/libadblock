#include "../parse_rule.hpp"

#include "core/element_hide_rule_base.hpp"
#include "core/filter_rule_base.hpp"
#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "rule/extended_element_hide_rule.hpp"

#include <gtest/gtest.h>

using namespace adblock;

TEST(Core_ElementHideRuleBase, Elementary)
{
    auto const rule = parse_rule<BasicElementHideRule>("##div"_r);
    ASSERT_TRUE(rule);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };
    ruleBase.put(*rule);

    auto const& result = ruleBase.query("http://www.adblock.org"_u);
    EXPECT_EQ("div { display: none !important } ", result);
}

TEST(Core_ElementHideRuleBase, Domained)
{
    auto const rule1 = parse_rule<BasicElementHideRule>("##div"_r);
    ASSERT_TRUE(rule1);

    auto const rule2 = parse_rule<BasicElementHideRule>("adblock.org##table"_r);
    ASSERT_TRUE(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    const auto &result = ruleBase.query("http://www.adblock.org"_u);
    // either is ok.
    //EXPECT_EQ("table, div { display: none !important }", result);
    EXPECT_EQ("div, table { display: none !important } ", result);
}

TEST(Core_ElementHideRuleBase, ExcludedByExceptionRule)
{
    auto const rule1 = parse_rule<BasicElementHideRule>("##div"_r);
    ASSERT_TRUE(rule1);

    auto const rule2 = parse_rule<ExceptionElementHideRule>("adblock.org#@#div"_r);
    ASSERT_TRUE(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    const auto &result = ruleBase.query("http://www.adblock.org"_u);
    EXPECT_EQ("", result);
}

TEST(Core_ElementHideRuleBase, ExcludedByGenericExceptionRule)
{
    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    auto const rule1 = parse_rule<BasicElementHideRule>("adblock.org##div"_r);
    ASSERT_TRUE(rule1);

    ruleBase.put(*rule1);

    {
        auto const& result = ruleBase.query("http://www.adblock.org"_u);
        EXPECT_EQ("div { display: none !important } ", result);
    }

    auto const rule2 = parse_rule<ExceptionElementHideRule>("#@#div"_r);
    ASSERT_TRUE(rule2);

    ruleBase.put(*rule2);

    {
        auto const& result = ruleBase.query("http://www.adblock.org"_u);
        EXPECT_EQ("", result);
    }
}

TEST(Core_ElementHideRuleBase, DomainMatchWithExceptionRuleButSelectorIsnTSame)
{
    auto const rule1 = parse_rule<BasicElementHideRule>("##div"_r);
    ASSERT_TRUE(rule1);

    auto const rule2 = parse_rule<ExceptionElementHideRule>("adblock.org#@#table"_r);
    ASSERT_TRUE(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    const auto &result = ruleBase.query("http://www.adblock.org"_u);
    EXPECT_EQ("div { display: none !important } ", result);
}

TEST(Core_ElementHideRuleBase, Clear)
{
    auto const rule1 = parse_rule<BasicElementHideRule>("##div"_r);
    ASSERT_TRUE(rule1);

    auto const rule2 = parse_rule<ExceptionElementHideRule>("adblock.org#@#table"_r);
    ASSERT_TRUE(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    auto stats = ruleBase.statistics();
    EXPECT_EQ(2, stats.get<size_t>("Total"));

    ruleBase.clear();

    stats = ruleBase.statistics();
    EXPECT_EQ(0, stats.get<size_t>("Total"));
}

TEST(Core_ElementHideRuleBase, ExtendedRule)
{
    auto const rule = parse_rule<ExtendedElementHideRule>("#?#div"_r);
    ASSERT_TRUE(rule);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };
    ruleBase.put(*rule);

    auto const& uri = "http://www.adblock.org"_u;

    auto const& result = ruleBase.query(uri);
    EXPECT_EQ("", result);

    auto const& rules = ruleBase.lookupExtendedRule(uri);
    ASSERT_EQ(1, rules.size());

    auto const& selector = rules[0]->cssSelector();
    EXPECT_TRUE("div"_r == selector) << selector;
}

TEST(Core_ElementHideRuleBase, DomainedExtendedRule)
{
    auto const rule1 = parse_rule<ExtendedElementHideRule>("#?#div"_r);
    ASSERT_TRUE(rule1);

    auto const rule2 = parse_rule<ExtendedElementHideRule>("adblock.org#?#table"_r);
    ASSERT_TRUE(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    auto const& uri = "http://www.adblock.org"_u;

    auto const& result = ruleBase.query(uri);
    EXPECT_EQ("", result);

    auto const& rules = ruleBase.lookupExtendedRule(uri);
    ASSERT_EQ(2, rules.size());

    EXPECT_TRUE("div"_r == rules[0]->cssSelector());
    EXPECT_TRUE("table"_r == rules[1]->cssSelector());
}

TEST(Core_ElementHideRuleBase, Extended_ExcludedByExceptionRule)
{
    auto const rule1 = parse_rule<ExtendedElementHideRule>("#?#div"_r);
    ASSERT_TRUE(rule1);

    auto const rule2 = parse_rule<ExceptionElementHideRule>("adblock.org#@#div"_r);
    ASSERT_TRUE(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    auto const& uri = "http://www.adblock.org"_u;

    auto const& result = ruleBase.query(uri);
    EXPECT_EQ("", result);

    auto const& rules = ruleBase.lookupExtendedRule(uri);
    EXPECT_TRUE(rules.empty());
}

TEST(Core_ElementHideRuleBase, ExtendedRule_DomainMatchWithExceptionRuleButSelectorIsnTSame)
{
    auto const rule1 = parse_rule<ExtendedElementHideRule>("#?#div"_r);
    ASSERT_TRUE(rule1);

    auto const rule2 = parse_rule<ExceptionElementHideRule>("adblock.org#@#table"_r);
    ASSERT_TRUE(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    auto const& uri = "http://www.adblock.org"_u;

    auto const& result = ruleBase.query(uri);
    EXPECT_TRUE(result.empty());

    auto const& rules = ruleBase.lookupExtendedRule(uri);
    ASSERT_EQ(1, rules.size());
    EXPECT_EQ("div"_r, rules[0]->cssSelector());
}

TEST(Core_ElementHideRuleBase, ExtendedRule_Clear)
{
    auto const rule1 = parse_rule<ExtendedElementHideRule>("#?#div"_r);
    ASSERT_TRUE(rule1);

    auto const rule2 = parse_rule<ExceptionElementHideRule>("adblock.org#@#table"_r);
    ASSERT_TRUE(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    auto stats = ruleBase.statistics();
    EXPECT_EQ(2, stats.get<size_t>("Total"));

    ruleBase.clear();

    stats = ruleBase.statistics();
    EXPECT_EQ(0, stats.get<size_t>("Total"));
}
