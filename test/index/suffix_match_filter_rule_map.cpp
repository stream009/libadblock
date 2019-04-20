#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "index/suffix_match_filter_rule_map.hpp"
#include "rule/basic_filter_rule.hpp"

#include <boost/range/algorithm.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Index_SuffixMatchFilterRuleMap, OneHit)
{
    auto const rule1 = parse_rule<BasicFilterRule>("adblock.org|"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("adblock.com|"_r);
    ASSERT_TRUE(rule2);

    SuffixMatchFilterRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto&& results = ruleSet.query("http://www.adblock.org"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(Index_SuffixMatchFilterRuleMap, MultipleHit)
{
    auto const rule1 = parse_rule<BasicFilterRule>("adblock.org|"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("www.adblock.org|"_r);
    ASSERT_TRUE(rule2);
    auto const rule3 = parse_rule<BasicFilterRule>("adblock.com|"_r);
    ASSERT_TRUE(rule3);

    SuffixMatchFilterRuleMap ruleSet;
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

TEST(Index_SuffixMatchFilterRuleMap, NoNit)
{
    auto const rule1 = parse_rule<BasicFilterRule>("google.com|"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("foo.adblock.org|"_r);
    ASSERT_TRUE(rule2);
    auto const rule3 = parse_rule<BasicFilterRule>("adblock.com|"_r);
    ASSERT_TRUE(rule3);

    SuffixMatchFilterRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);
    ruleSet.put(*rule3);

    auto&& results = ruleSet.query("http://www.adblock.org"_u);
    EXPECT_TRUE(results.empty());
}

TEST(Index_SuffixMatchFilterRuleMap, MultiToken)
{
    auto const rule1 = parse_rule<BasicFilterRule>("adblock.org*jpg|"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("adblock.com|"_r);
    ASSERT_TRUE(rule2);

    SuffixMatchFilterRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto&& results = ruleSet.query("http://www.adblock.org/top.jpg"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());

    results = ruleSet.query("http://www.google.com/top.jpg"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(Index_SuffixMatchFilterRuleMap, Clear)
{
    auto const rule1 = parse_rule<BasicFilterRule>("adblock.org*jpg|"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("adblock.com|"_r);
    ASSERT_TRUE(rule2);

    SuffixMatchFilterRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto stats = ruleSet.statistics();
    EXPECT_EQ(2, stats.get<size_t>("Number of values"));

    ruleSet.clear();

    stats = ruleSet.statistics();
    EXPECT_EQ(0, stats.get<size_t>("Number of values"));
}
