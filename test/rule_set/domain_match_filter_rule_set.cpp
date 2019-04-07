#include "type.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule_set/domain_match_filter_rule_set.hpp"

#include "option.hpp"
#include "../mock_context.hpp"

#include <memory>

#include <boost/range/algorithm.hpp>
#include <boost/optional.hpp>

#include <gtest/gtest.h>

using namespace adblock;

static std::shared_ptr<FilterRule>
make_rule(const StringRange &domain,
          const StringRange &pattern = boost::as_literal(""))
{
    return std::make_shared<BasicFilterRule>(
            std::make_shared<DomainMatchPattern>(domain, pattern, false),
            boost::none);
}

TEST(DomainMatchFilterRuleSet, OneHit)
{
    const auto &rule1 = make_rule("adblock.org"_r);
    const auto &rule2 = make_rule("ban"_r);

    DomainMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto &&results = ruleSet.query("http://www.adblock.org/ban"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(DomainMatchFilterRuleSet, MultipleHit)
{
    const auto &rule1 = make_rule("adblock.org"_r);
    const auto &rule2 = make_rule("www.*.org"_r);
    const auto &rule3 = make_rule("adblock.com"_r);

    DomainMatchFilterRuleSet ruleSet;
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

TEST(DomainMatchFilterRuleSet, NoHit)
{
    const auto &rule1 = make_rule("foo.adblock.org"_r);
    const auto &rule2 = make_rule("adblock.com"_r);

    DomainMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto &&results = ruleSet.query("http://www.adblock.org"_u);
    EXPECT_TRUE(results.empty());
}

TEST(DomainMatchFilterRuleSet, MultiToken)
{
    const auto &rule1 = make_rule("adblock*org"_r);
    const auto &rule2 = make_rule("ban"_r);

    DomainMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto &&results = ruleSet.query("http://www.adblock.org/ban"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule1, results.front());
}

TEST(DomainMatchFilterRuleSet, Clear)
{
    const auto &rule1 = make_rule("adblock*org"_r);
    const auto &rule2 = make_rule("ban"_r);

    DomainMatchFilterRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto stats = ruleSet.statistics();
    EXPECT_EQ(2, stats.get<size_t>("Number of values"));

    ruleSet.clear();

    stats = ruleSet.statistics();
    EXPECT_EQ(0, stats.get<size_t>("Number of values"));
}
