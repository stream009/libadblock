#include "type.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule_set/prefix_match_filter_rule_set.hpp"

#include <memory>

#include <boost/range/algorithm.hpp>

#include <gtest/gtest.h>

using namespace adblock;

static std::shared_ptr<FilterRule>
make_rule(StringRange const& pattern)
{
    return std::make_shared<BasicFilterRule>(
        std::make_unique<BasicMatchPattern>(pattern, true),
        std::vector<std::unique_ptr<Option>>()
    );
}

TEST(RuleSet_PrefixMatchFilterRuleSet, OneHit)
{
    const auto &rule1 = make_rule("http://www.adblock"_r);
    const auto &rule2 = make_rule("adblock"_r);

    PrefixMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto &&results = ruleSet.query("http://www.adblock.org"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(RuleSet_PrefixMatchFilterRuleSet, MultipleHit)
{
    const auto &rule1 = make_rule("http://www.adblock"_r);
    const auto &rule2 = make_rule("http://www.adblock.org"_r);
    const auto &rule3 = make_rule("http://www.google.com"_r);

    PrefixMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);
    ruleSet.put(*rule3);

    auto &&results = ruleSet.query("http://www.adblock.org"_u);
    ASSERT_EQ(2, results.size());
    namespace br = boost::range;
    EXPECT_TRUE(br::find(results, &*rule1) != results.end());
    EXPECT_TRUE(br::find(results, &*rule2) != results.end());
    EXPECT_TRUE(br::find(results, &*rule3) == results.end());
}

TEST(RuleSet_PrefixMatchFilterRuleSet, NoHit)
{
    const auto &rule1 = make_rule("http://www.adblock.net"_r);
    const auto &rule2 = make_rule("http://www.adblock.gov"_r);
    const auto &rule3 = make_rule("http://www.google.com"_r);

    PrefixMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);
    ruleSet.put(*rule3);

    auto &&results = ruleSet.query("http://www.adblock.org"_u);
    EXPECT_TRUE(results.empty());
}

TEST(RuleSet_PrefixMatchFilterRuleSet, MultiToken)
{
    const auto &rule1 = make_rule("http://www.adblock*jpg"_r);
    const auto &rule2 = make_rule("adblock"_r);

    PrefixMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto &&results = ruleSet.query("http://www.adblock.org"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(RuleSet_PrefixMatchFilterRuleSet, Clear)
{
    const auto &rule1 = make_rule("http://www.adblock*jpg"_r);
    const auto &rule2 = make_rule("adblock"_r);

    PrefixMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto stats = ruleSet.statistics();
    EXPECT_EQ(2, stats.get<size_t>("Number of values"));

    ruleSet.clear();

    stats = ruleSet.statistics();
    EXPECT_EQ(0, stats.get<size_t>("Number of values"));
}
