#include "../parse_rule.hpp"

#include "core/json.hpp"
#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "index/domained_element_hide_rule_map.hpp"
#include "rule/basic_element_hide_rule.hpp"

#include <boost/range/algorithm.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Index_DomainedElementHideRuleMap, Basic)
{
    auto const rule = parse_rule<BasicElementHideRule>("adblock.org##div"_r);
    ASSERT_TRUE(rule);

    DomainedElementHideRuleMap ruleSet;
    ruleSet.put(*rule);

    auto&& results = ruleSet.query("http://www.adblock.org/ban"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule, results.front());

    results = ruleSet.query("http://www.adblockplus.org/"_u);
    EXPECT_TRUE(results.empty());
}

TEST(Index_DomainedElementHideRuleMap, MultipleDomain)
{
    auto const rule = parse_rule<BasicElementHideRule>("adblock.org,google.com##div"_r);
    ASSERT_TRUE(rule);

    DomainedElementHideRuleMap ruleSet;
    ruleSet.put(*rule);

    auto&& results = ruleSet.query("http://www.adblock.org/ban"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule, results.front());

    results = ruleSet.query("http://www.google.com/something"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule, results.front());

    results = ruleSet.query("http://www.adblockplus.org/"_u);
    EXPECT_TRUE(results.empty());
}

TEST(Index_DomainedElementHideRuleMap, ExcludedByInverseDomain)
{
    auto const rule = parse_rule<ElementHideRule>("adblock.org,~sub.adblock.org##div"_r);
    ASSERT_TRUE(rule);

    DomainedElementHideRuleMap ruleSet;
    ruleSet.put(*rule);

    auto&& results = ruleSet.query("http://www.adblock.org/ban"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule, results.front());

    results = ruleSet.query("http://www.sub.adblock.org/other"_u);
    EXPECT_TRUE(results.empty());
}

TEST(Index_DomainedElementHideRuleMap, MultipleHit)
{
    auto const rule1 = parse_rule<BasicElementHideRule>("adblock.org##div"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicElementHideRule>("sub.adblock.org##table"_r);
    ASSERT_TRUE(rule2);

    DomainedElementHideRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto&& results = ruleSet.query("http://www.adblock.org/something"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());

    results = ruleSet.query("http://sub.adblock.org/ban"_u);
    ASSERT_EQ(2, results.size());
    namespace br = boost::range;
    EXPECT_TRUE(br::find(results, &*rule1) != results.end());
    EXPECT_TRUE(br::find(results, &*rule2) != results.end());

    results = ruleSet.query("http://www.adblockplus.org/"_u);
    EXPECT_TRUE(results.empty());
}

TEST(Index_DomainedElementHideRuleMap, ExcludeOnlyRule)
{
    auto const rule = parse_rule<BasicElementHideRule>("~adblock.org##div"_r);
    ASSERT_TRUE(rule);

    DomainedElementHideRuleMap ruleSet;
    ruleSet.put(*rule);

    auto&& results = ruleSet.query("http://www.adblock.org/something"_u);
    EXPECT_TRUE(results.empty());

    results = ruleSet.query("http://www.adblockplus.org/"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule, results.front());

    results = ruleSet.query("http://www.google.com/"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule, results.front());
}

TEST(Index_DomainedElementHideRuleMap, Statistics)
{
    auto const rule1 = parse_rule<BasicElementHideRule>("adblock.org##div"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicElementHideRule>("sub.adblock.org##table"_r);
    ASSERT_TRUE(rule2);
    auto const rule3 = parse_rule<BasicElementHideRule>("~adblock.org##div"_r);
    ASSERT_TRUE(rule3);

    DomainedElementHideRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);
    ruleSet.put(*rule3);

    auto const& stats = ruleSet.statistics();

    EXPECT_EQ(1, to_number(stats.at("Number of leaf")));
    EXPECT_EQ(2, to_number(stats.at("Number of branch")));
    EXPECT_EQ(3, to_number(stats.at("Number of nodes")));
    EXPECT_EQ(2, to_number(stats.at("Number of values")));

    auto child = stats.at("Branches by children").get_object();
    EXPECT_EQ(1, child.size());
    EXPECT_EQ(2, to_number(child["1"]));

    child = stats.at("Branches by level").get_object();
    EXPECT_EQ(2, child.size());
    EXPECT_EQ(1, to_number(child["0"]));
    EXPECT_EQ(1, to_number(child["1"]));

    child = stats.at("Leaves by level").get_object();
    EXPECT_EQ(1, child.size());
    EXPECT_EQ(1, to_number(child["2"]));

    child = stats.at("Nodes by values").get_object();
    EXPECT_EQ(2, child.size());
    EXPECT_EQ(1, to_number(child["0"]));
    EXPECT_EQ(2, to_number(child["1"]));

    EXPECT_EQ(1, to_number(stats.at("Exception only rules")));
}

TEST(Index_DomainedElementHideRuleMap, Clear)
{
    auto const rule1 = parse_rule<BasicElementHideRule>("adblock.org##div"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicElementHideRule>("sub.adblock.org##table"_r);
    ASSERT_TRUE(rule2);
    auto const rule3 = parse_rule<BasicElementHideRule>("~adblock.org##div"_r);
    ASSERT_TRUE(rule3);

    DomainedElementHideRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);
    ruleSet.put(*rule3);

    auto stats = ruleSet.statistics();
    EXPECT_EQ(2, to_number(stats.at("Number of values")));
    EXPECT_EQ(1, to_number(stats.at("Exception only rules")));

    ruleSet.clear();

    stats = ruleSet.statistics();
    EXPECT_EQ(0, to_number(stats.at("Number of values")));
    EXPECT_EQ(0, to_number(stats.at("Exception only rules")));
}
