#include "type.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule_set/substring_match_filter_rule_set.hpp"

#include <boost/range/algorithm.hpp>

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

static std::shared_ptr<FilterRule>
make_rule(const StringRange &pattern)
{
    return std::make_shared<BasicFilterRule>(
            std::make_shared<BasicMatchPattern>(pattern), boost::none);
}

TEST(SubstringMatchFilterRuleSet, Basic)
{
    const auto &rule1 = make_rule("adblock"_r);
    const auto &rule2 = make_rule("adblockplus"_r);

    SubstringMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto &&results = ruleSet.query("http://www.adblock.org"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(SubstringMatchFilterRuleSet, MultipleHit)
{
    const auto &rule1 = make_rule("adblock"_r);
    const auto &rule2 = make_rule("adblock*g"_r);
    const auto &rule3 = make_rule("ad*org"_r);
    const auto &rule4 = make_rule("adblockplus"_r);

    SubstringMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);
    ruleSet.put(*rule3);
    ruleSet.put(*rule4);

    auto &&results = ruleSet.query("http://www.adblock.org"_u);
    ASSERT_EQ(3, results.size());
    namespace br = boost::range;
    EXPECT_TRUE(br::find(results, &*rule1) != results.end());
    EXPECT_TRUE(br::find(results, &*rule2) != results.end());
    EXPECT_TRUE(br::find(results, &*rule3) != results.end());
    EXPECT_TRUE(br::find(results, &*rule4) == results.end());
}

TEST(SubstringMatchFilterRuleSet, NoHit)
{
    const auto &rule1 = make_rule("adblocks"_r);
    const auto &rule2 = make_rule("google.com"_r);

    SubstringMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto &&results = ruleSet.query("http://www.adblock.org"_u);
    EXPECT_TRUE(results.empty());
}

TEST(SubstringMatchFilterRuleSet, MultiToken)
{
    const auto &rule1 = make_rule("adblock*jpg"_r);
    const auto &rule2 = make_rule("adblockplus"_r);

    SubstringMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto &&results = ruleSet.query("http://www.adblock.org/top.png"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(SubstringMatchFilterRuleSet, Clear)
{
    const auto &rule1 = make_rule("adblock*jpg"_r);
    const auto &rule2 = make_rule("adblockplus"_r);

    SubstringMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto stats = ruleSet.statistics();
    EXPECT_EQ(2, stats.get<size_t>("Number of values"));

    ruleSet.clear();

    stats = ruleSet.statistics();
    EXPECT_EQ(0, stats.get<size_t>("Number of values"));
}
