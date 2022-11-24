#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "core/filter_rule_base.hpp"
#include "core/json.hpp"
#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"

#include <gtest/gtest.h>

using namespace adblock;

TEST(Core_FilterRuleBase, Basic)
{
    FilterRuleBase rb;
    MockContext cxt;

    auto const rule = parse_rule<BasicFilterRule>("adblock"_r);
    ASSERT_TRUE(rule);

    rb.put(*rule);

    {
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule));
    }
    {
        auto const& rv = rb.query("http://www.google.com"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == nullptr);
    }
}

TEST(Core_FilterRuleBase, Domain)
{
    FilterRuleBase rb;
    MockContext cxt;

    auto const rule = parse_rule<BasicFilterRule>("||adblock.org"_r);
    ASSERT_TRUE(rule);

    rb.put(*rule);

    {
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule));
    }
    {
        auto const& rv = rb.query("http://www.google.com"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == nullptr);
    }
}

TEST(Core_FilterRuleBase, Regex)
{
    FilterRuleBase rb;
    MockContext cxt;

    auto const rule = parse_rule<BasicFilterRule>(R"(/.*adblock.*/)"_r);
    ASSERT_TRUE(rule);

    rb.put(*rule);

    {
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule));
    }
    {
        auto const& rv = rb.query("http://www.google.com"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == nullptr);
    }
}

TEST(Core_FilterRuleBase, ExceptionBasic)
{
    FilterRuleBase rb;
    MockContext cxt;

    auto const rule1 = parse_rule<BasicFilterRule>("org"_r);
    ASSERT_TRUE(rule1);

    rb.put(*rule1);

    auto const rule2 = parse_rule<ExceptionFilterRule>("@@adblock"_r);
    ASSERT_TRUE(rule2);

    rb.put(*rule2);

    {
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule2));
    }
    {
        auto const& rv = rb.query("http://www.foobar.org"_u, cxt);
        EXPECT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1)) << rv.second;
    }
}

TEST(Core_FilterRuleBase, ExceptionDomain)
{
    FilterRuleBase rb;
    MockContext cxt;

    auto const rule1 = parse_rule<BasicFilterRule>("org"_r);
    ASSERT_TRUE(rule1);

    rb.put(*rule1);

    auto const rule2 = parse_rule<ExceptionFilterRule>("@@adblock.org"_r);
    ASSERT_TRUE(rule2);

    rb.put(*rule2);

    {
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule2));
    }
    {
        auto const& rv = rb.query("http://www.foobar.org"_u, cxt);
        EXPECT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1)) << rv.second;
    }
}

TEST(Core_FilterRuleBase, ExceptionRegex)
{
    FilterRuleBase rb;
    MockContext cxt;

    auto const rule1 = parse_rule<BasicFilterRule>("org"_r);
    ASSERT_TRUE(rule1);

    rb.put(*rule1);

    auto const rule2 = parse_rule<ExceptionFilterRule>(R"(@@/.*adblock.*/)"_r);
    ASSERT_TRUE(rule2);

    rb.put(*rule2);

    {
        auto const& rv = rb.query("http://www.adblock.org"_u, cxt);
        ASSERT_FALSE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule2));
    }
    {
        auto const& rv = rb.query("http://www.foobar.org"_u, cxt);
        EXPECT_TRUE(rv.first);
        EXPECT_TRUE(rv.second == &(*rule1)) << rv.second;
    }
}

TEST(Core_FilterRuleBase, Clear)
{
    FilterRuleBase rb;

    auto const rule1 = parse_rule<BasicFilterRule>("adblock"_r);
    ASSERT_TRUE(rule1);

    rb.put(*rule1);

    auto const rule2 = parse_rule<ExceptionFilterRule>(R"(@@/.*adblock\.org.*/)"_r);
    ASSERT_TRUE(rule2);

    rb.put(*rule2);

    auto const rule3 = parse_rule<FilterRule>("/adsense/*"_r);
    ASSERT_TRUE(rule3);

    rb.put(*rule3);

    auto const rule4 = parse_rule<FilterRule>("@@||www.google.*/adsense/$~third-party"_r);
    ASSERT_TRUE(rule4);

    rb.put(*rule4);

    const auto &before = rb.statistics();
    EXPECT_EQ(4, to_number(before.at("Total")));
    EXPECT_EQ(2, to_number(before.at("Basic match pattern")));
    EXPECT_EQ(2, to_number(before.at("Exception match pattern")));

    rb.clear();

    const auto &after = rb.statistics();
    EXPECT_EQ(0, to_number(after.at("Total")));
    EXPECT_EQ(0, to_number(after.at("Basic match pattern")));
    EXPECT_EQ(0, to_number(after.at("Exception match pattern")));
}
