#include "../parse_rule.hpp"

#include "rule/basic_filter_rule.hpp"
#include "index/prefix_match_filter_rule_map.hpp"
#include "core/type.hpp"

#include <boost/range/algorithm.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Index_PrefixMatchFilterRuleMap, OneHit)
{
    auto const rule1 = parse_rule<BasicFilterRule>("|http://www.adblock"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("|adblock"_r);
    ASSERT_TRUE(rule2);

    PrefixMatchFilterRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto&& results = ruleSet.query("http://www.adblock.org"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(Index_PrefixMatchFilterRuleMap, MultipleHit)
{
    auto const rule1 = parse_rule<BasicFilterRule>("|http://www.adblock"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("|http://www.adblock.org"_r);
    ASSERT_TRUE(rule2);
    auto const rule3 = parse_rule<BasicFilterRule>("|http://www.google.com"_r);
    ASSERT_TRUE(rule3);

    PrefixMatchFilterRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);
    ruleSet.put(*rule3);

    auto&& results = ruleSet.query("http://www.adblock.org"_u);
    ASSERT_EQ(2, results.size());
    namespace br = boost::range;
    EXPECT_TRUE(br::find(results, &*rule1) != results.end());
    EXPECT_TRUE(br::find(results, &*rule2) != results.end());
    EXPECT_TRUE(br::find(results, &*rule3) == results.end());
}

TEST(Index_PrefixMatchFilterRuleMap, NoHit)
{
    auto const rule1 = parse_rule<BasicFilterRule>("|http://www.adblock.net"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("|http://www.adblock.gov"_r);
    ASSERT_TRUE(rule2);
    auto const rule3 = parse_rule<BasicFilterRule>("|http://www.google.com"_r);
    ASSERT_TRUE(rule3);

    PrefixMatchFilterRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);
    ruleSet.put(*rule3);

    auto&& results = ruleSet.query("http://www.adblock.org"_u);
    EXPECT_TRUE(results.empty());
}

TEST(Index_PrefixMatchFilterRuleMap, MultiToken)
{
    auto const rule1 = parse_rule<BasicFilterRule>("|http://www.adblock*jpg"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("|adblock"_r);
    ASSERT_TRUE(rule2);

    PrefixMatchFilterRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto&& results = ruleSet.query("http://www.adblock.org"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(Index_PrefixMatchFilterRuleMap, Clear)
{
    auto const rule1 = parse_rule<BasicFilterRule>("|http://www.adblock*jpg"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("|adblock"_r);
    ASSERT_TRUE(rule2);

    PrefixMatchFilterRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto stats = ruleSet.statistics();
    EXPECT_EQ(2, stats.get<size_t>("Number of values"));

    ruleSet.clear();

    stats = ruleSet.statistics();
    EXPECT_EQ(0, stats.get<size_t>("Number of values"));
}
