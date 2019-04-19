#include "../parse_rule.hpp"

#include "rule/basic_filter_rule.hpp"
#include "index/domain_match_filter_rule_map.hpp"
#include "type.hpp"

#include <boost/range/algorithm.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Index_DomainMatchFilterRuleMap, OneHit)
{
    auto const rule1 = parse_rule<BasicFilterRule>("||adblock.org"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("||ban"_r);
    ASSERT_TRUE(rule2);

    DomainMatchFilterRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto&& results = ruleSet.query("http://www.adblock.org/ban"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(Index_DomainMatchFilterRuleMap, MultipleHit)
{
    auto const rule1 = parse_rule<BasicFilterRule>("||adblock.org"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("||www.*.org"_r);
    ASSERT_TRUE(rule2);
    auto const rule3 = parse_rule<BasicFilterRule>("||adblock.com"_r);
    ASSERT_TRUE(rule3);

    DomainMatchFilterRuleMap ruleSet;
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

TEST(Index_DomainMatchFilterRuleMap, NoHit)
{
    auto const rule1 = parse_rule<BasicFilterRule>("||foo.adblock.org"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("||adblock.com"_r);
    ASSERT_TRUE(rule2);

    DomainMatchFilterRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto&& results = ruleSet.query("http://www.adblock.org"_u);
    EXPECT_TRUE(results.empty());
}

TEST(Index_DomainMatchFilterRuleMap, MultiToken)
{
    auto const rule1 = parse_rule<BasicFilterRule>("||adblock*org"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("||ban"_r);
    ASSERT_TRUE(rule2);

    DomainMatchFilterRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto&& results = ruleSet.query("http://www.adblock.org/ban"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(Index_DomainMatchFilterRuleMap, Clear)
{
    auto const rule1 = parse_rule<BasicFilterRule>("||adblock*org"_r);
    ASSERT_TRUE(rule1);
    auto const rule2 = parse_rule<BasicFilterRule>("||ban"_r);
    ASSERT_TRUE(rule2);

    DomainMatchFilterRuleMap ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto stats = ruleSet.statistics();
    EXPECT_EQ(2, stats.get<size_t>("Number of values"));

    ruleSet.clear();

    stats = ruleSet.statistics();
    EXPECT_EQ(0, stats.get<size_t>("Number of values"));
}
