#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"

#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Rule_BasicFilterRule, Basic)
{
    auto const& line = "Adblock"_r;

    auto const rule = parse_rule<BasicFilterRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_EQ(0, rule->numOptions());

    MockContext const context {};
    auto const& uri = "http://adblock.org"_u;

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Rule_BasicFilterRule, WithOption)
{
    auto const& line = "adblock$script"_r;

    auto const rule = parse_rule<BasicFilterRule>(line);
    ASSERT_TRUE(rule);

    struct ScriptContext : MockContext {
        bool fromScriptTag() const { return true; }
    } context {};

    auto const& uri = "http://adblock.org"_u;

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Rule_BasicFilterRule, CaseSensitiveMatch)
{
    auto const& line = "Adblock$match-case"_r;

    auto const rule = parse_rule<BasicFilterRule>(line);
    ASSERT_TRUE(rule);

    const MockContext context {};
    {
        auto const& uri = "http://adblock.org"_u;
        EXPECT_FALSE(rule->match(uri, context));
    }
    {
        auto const& uri = "http://Adblock.org"_u;
        EXPECT_TRUE(rule->match(uri, context));
    }
}

TEST(Rule_ExceptionFilterRule, Basic)
{
    auto const& line = "@@adblock"_r;

    auto const rule = parse_rule<ExceptionFilterRule>(line);
    ASSERT_TRUE(rule);

    MockContext const context {};
    auto const& uri = "http://adblock.org"_u;

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Rule_ExceptionFilterRule, WithOption)
{
    auto const& line = "@@adblock$script"_r;

    auto const rule = parse_rule<ExceptionFilterRule>(line);
    ASSERT_TRUE(rule);

    struct ScriptContext : MockContext {
        bool fromScriptTag() const { return true; }
    } context {};

    auto const& uri = "http://adblock.org"_u;

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Rule_ExceptionFilterRule, CaseSensitiveMatch)
{
    auto const& line = "@@Adblock$match-case"_r;

    auto const rule = parse_rule<ExceptionFilterRule>(line);
    ASSERT_TRUE(rule);

    const MockContext context {};
    {
        auto const& uri = "http://adblock.org"_u;
        EXPECT_FALSE(rule->match(uri, context));
    }
    {
        auto const& uri = "http://Adblock.org"_u;
        EXPECT_TRUE(rule->match(uri, context));
    }
}
