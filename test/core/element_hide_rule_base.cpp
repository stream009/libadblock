#include "../parse_rule.hpp"

#include "core/element_hide_rule_base.hpp"
#include "core/filter_rule_base.hpp"
#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "rule/extended_element_hide_rule.hpp"

#include <algorithm>

#include <gtest/gtest.h>

using namespace adblock;

template<typename T, typename U>
static bool
contains(std::vector<T> const& vec, U const& value)
{
    return std::find_if(
        vec.begin(), vec.end(),
        [&](auto& v) { return v == value; }
    ) != vec.end();
}

TEST(Core_ElementHideRuleBase, Elementary)
{
    auto const rule = parse_rule<BasicElementHideRule>("##div"_r);
    ASSERT_TRUE(rule);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };
    ruleBase.put(*rule);

    auto const& rules = ruleBase.lookupRules("http://www.adblock.org"_u);
    ASSERT_EQ(1, rules.size());
    EXPECT_EQ(rule.get(), rules[0]);
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

    auto const& rules = ruleBase.lookupRules("http://www.adblock.org"_u);
    ASSERT_EQ(2, rules.size());
    EXPECT_TRUE(contains(rules, rule1.get()));
    EXPECT_TRUE(contains(rules, rule2.get()));
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

    auto const& rules = ruleBase.lookupRules("http://www.adblock.org"_u);
    ASSERT_TRUE(rules.empty());
}

TEST(Core_ElementHideRuleBase, ExcludedByGenericExceptionRule)
{
    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    auto const rule1 = parse_rule<BasicElementHideRule>("adblock.org##div"_r);
    ASSERT_TRUE(rule1);

    ruleBase.put(*rule1);

    {
        auto const& rules = ruleBase.lookupRules("http://www.adblock.org"_u);
        ASSERT_EQ(1, rules.size());
        EXPECT_EQ(rule1.get(), rules[0]);
    }

    auto const rule2 = parse_rule<ExceptionElementHideRule>("#@#div"_r);
    ASSERT_TRUE(rule2);

    ruleBase.put(*rule2);

    {
        auto const& rules = ruleBase.lookupRules("http://www.adblock.org"_u);
        ASSERT_TRUE(rules.empty());
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

    auto const& rules = ruleBase.lookupRules("http://www.adblock.org"_u);
    ASSERT_EQ(1, rules.size());
    EXPECT_EQ(rule1.get(), rules[0]);
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

    {
        auto const& rules = ruleBase.lookupRules(uri);
        ASSERT_TRUE(rules.empty());
    }
    {
        auto const& rules = ruleBase.lookupExtendedRules(uri);
        ASSERT_EQ(1, rules.size());
        EXPECT_EQ(rule.get(), rules[0]);
    }
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

    {
        auto const& rules = ruleBase.lookupRules(uri);
        ASSERT_TRUE(rules.empty());
    }
    {
        auto const& rules = ruleBase.lookupExtendedRules(uri);
        ASSERT_EQ(2, rules.size());
        EXPECT_TRUE(contains(rules, rule1.get()));
        EXPECT_TRUE(contains(rules, rule2.get()));
    }
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

    {
        auto const& rules = ruleBase.lookupRules(uri);
        ASSERT_TRUE(rules.empty());
    }
    {
        auto const& rules = ruleBase.lookupExtendedRules(uri);
        ASSERT_TRUE(rules.empty());
    }
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
    {
        auto const& rules = ruleBase.lookupRules(uri);
        ASSERT_TRUE(rules.empty());
    }
    {
        auto const& rules = ruleBase.lookupExtendedRules(uri);
        ASSERT_EQ(1, rules.size());
        EXPECT_TRUE(contains(rules, rule1.get()));
    }
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
