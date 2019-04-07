#include "type.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule_set/domained_element_hide_rule_set.hpp"

#include <memory>

#include <boost/optional.hpp>
#include <boost/range/algorithm.hpp>

#include <gtest/gtest.h>

using namespace adblock;

using Domains = ElementHideRule::Domains;

static std::shared_ptr<ElementHideRule>
make_rule(const StringRange &cssSelector,
          const boost::optional<Domains> &domains)
{
    return std::make_shared<BasicElementHideRule>(cssSelector, domains);
}

TEST(DomainedElementHideRuleSet, Basic)
{
    const auto &rule = make_rule("div"_r, Domains { "adblock.org"_r });
    assert(rule);

    DomainedElementHideRuleSet ruleSet;
    ruleSet.put(*rule);

    auto &&results = ruleSet.query("http://www.adblock.org/ban"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule, results.front());

    results = ruleSet.query("http://www.adblockplus.org/"_u);
    EXPECT_TRUE(results.empty());
}

TEST(DomainedElementHideRuleSet, MultipleDomain)
{
    const Domains &domains {
        "adblock.org"_r,
        "google.com"_r,
    };
    const StringRange &cssSelector = "div"_r;

    const auto &rule = make_rule(cssSelector, domains);
    assert(rule);

    DomainedElementHideRuleSet ruleSet;
    ruleSet.put(*rule);

    auto &&results = ruleSet.query("http://www.adblock.org/ban"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule, results.front());

    results = ruleSet.query("http://www.google.com/something"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule, results.front());

    results = ruleSet.query("http://www.adblockplus.org/"_u);
    EXPECT_TRUE(results.empty());
}

TEST(DomainedElementHideRuleSet, ExcludedByInverseDomain)
{
    const Domains &domains {
        "adblock.org"_r,
        "~sub.adblock.org"_r,
    };
    const StringRange &cssSelector = "div"_r;

    const auto &rule = make_rule(cssSelector, domains);
    assert(rule);

    DomainedElementHideRuleSet ruleSet;
    ruleSet.put(*rule);

    auto &&results = ruleSet.query("http://www.adblock.org/ban"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule, results.front());

    results = ruleSet.query("http://www.sub.adblock.org/other"_u);
    EXPECT_TRUE(results.empty());
}

TEST(DomainedElementHideRuleSet, MultipleHit)
{
    const auto &rule1 = make_rule("div"_r, Domains { "adblock.org"_r, });
    assert(rule1);
    const auto &rule2 = make_rule("table"_r, Domains { "sub.adblock.org"_r, });
    assert(rule2);

    DomainedElementHideRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);

    auto &&results = ruleSet.query("http://www.adblock.org/something"_u);
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

TEST(DomainedElementHideRuleSet, ExcludeOnlyRule)
{
    const auto &rule = make_rule("div"_r, Domains { "~adblock.org"_r, });
    assert(rule);

    DomainedElementHideRuleSet ruleSet;
    ruleSet.put(*rule);

    auto &&results = ruleSet.query("http://www.adblock.org/something"_u);
    EXPECT_TRUE(results.empty());

    results = ruleSet.query("http://www.adblockplus.org/"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule, results.front());

    results = ruleSet.query("http://www.google.com/"_u);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(&*rule, results.front());
}

TEST(DomainedElementHideRuleSet, Statistics)
{
    const auto &rule1 = make_rule("div"_r, Domains { "adblock.org"_r, });
    assert(rule1);
    const auto &rule2 = make_rule("table"_r, Domains { "sub.adblock.org"_r, });
    assert(rule2);
    const auto &rule3 = make_rule("div"_r, Domains { "~adblock.org"_r, });
    assert(rule3);

    DomainedElementHideRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);
    ruleSet.put(*rule3);

    const auto &stats = ruleSet.statistics();

    EXPECT_EQ(1, stats.get<size_t>("Number of leaf"));
    EXPECT_EQ(2, stats.get<size_t>("Number of branch"));
    EXPECT_EQ(3, stats.get<size_t>("Number of nodes"));
    EXPECT_EQ(2, stats.get<size_t>("Number of values"));

    auto child = stats.get_child("Branches by children");
    EXPECT_EQ(1, child.size());
    EXPECT_EQ(2, child.get<size_t>("1"));

    child = stats.get_child("Branches by level");
    EXPECT_EQ(2, child.size());
    EXPECT_EQ(1, child.get<size_t>("0"));
    EXPECT_EQ(1, child.get<size_t>("1"));

    child = stats.get_child("Leaves by level");
    EXPECT_EQ(1, child.size());
    EXPECT_EQ(1, child.get<size_t>("2"));

    child = stats.get_child("Nodes by values");
    EXPECT_EQ(2, child.size());
    EXPECT_EQ(1, child.get<size_t>("0"));
    EXPECT_EQ(2, child.get<size_t>("1"));

    EXPECT_EQ(1, stats.get<size_t>("Exception only rules"));
}

TEST(DomainedElementHideRuleSet, Clear)
{
    const auto &rule1 = make_rule("div"_r, Domains { "adblock.org"_r, });
    assert(rule1);
    const auto &rule2 = make_rule("table"_r, Domains { "sub.adblock.org"_r, });
    assert(rule2);
    const auto &rule3 = make_rule("div"_r, Domains { "~adblock.org"_r, });
    assert(rule3);

    DomainedElementHideRuleSet ruleSet;
    ruleSet.put(*rule1);
    ruleSet.put(*rule2);
    ruleSet.put(*rule3);

    auto stats = ruleSet.statistics();
    EXPECT_EQ(2, stats.get<size_t>("Number of values"));
    EXPECT_EQ(1, stats.get<size_t>("Exception only rules"));

    ruleSet.clear();

    stats = ruleSet.statistics();
    EXPECT_EQ(0, stats.get<size_t>("Number of values"));
    EXPECT_EQ(0, stats.get<size_t>("Exception only rules"));
}
