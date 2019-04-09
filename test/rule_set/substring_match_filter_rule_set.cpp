#include "../parse_rule.hpp"

#include "rule/basic_filter_rule.hpp"
#include "rule_set/substring_match_filter_rule_set.hpp"
#include "type.hpp"

#include <boost/range/algorithm.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(RuleSet_SubstringMatchFilterRuleSet, Basic)
{
    auto const rule1 = parse_rule<BasicFilterRule>("adblock"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("adblockplus"_r);
    ASSERT_TRUE(rule2);

    SubstringMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto&& results = ruleSet.query("http://www.adblock.org"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(RuleSet_SubstringMatchFilterRuleSet, MultipleHit)
{
    auto const rule1 = parse_rule<BasicFilterRule>("adblock"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("adblock*g"_r);
    ASSERT_TRUE(rule2);
    auto const rule3 = parse_rule<BasicFilterRule>("ad*org"_r);
    ASSERT_TRUE(rule3);
    auto const rule4 = parse_rule<BasicFilterRule>("adblockplus"_r);
    ASSERT_TRUE(rule4);

    SubstringMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);
    ruleSet.put(*rule3);
    ruleSet.put(*rule4);

    auto&& results = ruleSet.query("http://www.adblock.org"_u);
    ASSERT_EQ(3, results.size());
    namespace br = boost::range;
    EXPECT_TRUE(br::find(results, &*rule1) != results.end());
    EXPECT_TRUE(br::find(results, &*rule2) != results.end());
    EXPECT_TRUE(br::find(results, &*rule3) != results.end());
    EXPECT_TRUE(br::find(results, &*rule4) == results.end());
}

TEST(RuleSet_SubstringMatchFilterRuleSet, NoHit)
{
    auto const rule1 = parse_rule<BasicFilterRule>("adblocks"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("google.com"_r);
    ASSERT_TRUE(rule2);

    SubstringMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto&& results = ruleSet.query("http://www.adblock.org"_u);
    EXPECT_TRUE(results.empty());
}

TEST(RuleSet_SubstringMatchFilterRuleSet, MultiToken)
{
    auto const rule1 = parse_rule<BasicFilterRule>("adblock*jpg"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("adblockplus"_r);
    ASSERT_TRUE(rule2);

    SubstringMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto&& results = ruleSet.query("http://www.adblock.org/top.png"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(RuleSet_SubstringMatchFilterRuleSet, Clear)
{
    auto const rule1 = parse_rule<BasicFilterRule>("adblock*jpg"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("adblockplus"_r);
    ASSERT_TRUE(rule2);

    SubstringMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto stats = ruleSet.statistics();
    EXPECT_EQ(2, stats.get<size_t>("Number of values"));

    ruleSet.clear();

    stats = ruleSet.statistics();
    EXPECT_EQ(0, stats.get<size_t>("Number of values"));
}
