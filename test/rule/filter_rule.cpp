#include "type.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "option/script_option.hpp"
#include "option/match_case_option.hpp"

#include "../mock_context.hpp"

#include <memory>

#include <boost/range/iterator_range.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Rule_BasicFilterRule, Basic)
{
    auto pattern =
        std::make_unique<BasicMatchPattern>("Adblock"_r);

    BasicFilterRule rule { std::move(pattern), {} };

    EXPECT_TRUE(rule.options().empty());

    MockContext const context {};
    auto const& uri = "http://adblock.org"_u;

    EXPECT_TRUE(rule.match(uri, context));
}

TEST(Rule_BasicFilterRule, WithOption)
{
    auto pattern =
        std::make_unique<BasicMatchPattern>("adblock"_r);

    FilterRule::Options theOptions;
    theOptions.push_back(std::make_unique<ScriptOption>(false));

    BasicFilterRule rule { std::move(pattern), std::move(theOptions) };

    auto const& options = rule.options();
    ASSERT_EQ(1, options.size());

    auto* const option = options[0];
    ASSERT_TRUE(dynamic_cast<ScriptOption const*>(option));

    struct ScriptContext : MockContext {
        bool fromScriptTag() const { return true; }
    } context {};

    auto const& uri = "http://adblock.org"_u;

    EXPECT_TRUE(rule.match(uri, context));
}

TEST(Rule_BasicFilterRule, CaseSensitiveMatch)
{
    auto pattern =
        std::make_unique<BasicMatchPattern>("Adblock"_r);

    FilterRule::Options theOptions;
    theOptions.push_back(std::make_unique<MatchCaseOption>());

    BasicFilterRule const rule { std::move(pattern), std::move(theOptions) };

    const MockContext context {};
    {
        auto const& uri = "http://adblock.org"_u;
        EXPECT_FALSE(rule.match(uri, context));
    }
    {
        auto const& uri = "http://Adblock.org"_u;
        EXPECT_TRUE(rule.match(uri, context));
    }
}

TEST(Rule_ExceptionFilterRule, Basic)
{
    auto pattern =
        std::make_unique<BasicMatchPattern>("Adblock"_r);

    ExceptionFilterRule const rule { std::move(pattern), {} };

    EXPECT_TRUE(rule.options().empty());

    MockContext const context {};
    auto const& uri = "http://adblock.org"_u;

    EXPECT_TRUE(rule.match(uri, context));
}

TEST(Rule_ExceptionFilterRule, WithOption)
{
    auto pattern =
        std::make_unique<BasicMatchPattern>("adblock"_r);

    FilterRule::Options theOptions;
    theOptions.push_back(std::make_unique<ScriptOption>(false));

    ExceptionFilterRule const rule { std::move(pattern), std::move(theOptions) };

    auto const& options = rule.options();
    ASSERT_EQ(1, options.size());

    auto* const option = options[0];
    ASSERT_TRUE(dynamic_cast<ScriptOption const*>(option));

    struct ScriptContext : MockContext {
        bool fromScriptTag() const { return true; }
    } context {};

    auto const& uri = "http://adblock.org"_u;

    EXPECT_TRUE(rule.match(uri, context));
}

TEST(Rule_ExceptionFilterRule, CaseSensitiveMatch)
{
    auto pattern =
        std::make_unique<BasicMatchPattern>("Adblock"_r);

    FilterRule::Options theOptions;
    theOptions.push_back(std::make_unique<MatchCaseOption>());

    ExceptionFilterRule const rule { std::move(pattern), std::move(theOptions) };
    const MockContext context {};
    {
        auto const& uri = "http://adblock.org"_u;
        EXPECT_FALSE(rule.match(uri, context));
    }
    {
        auto const& uri = "http://Adblock.org"_u;
        EXPECT_TRUE(rule.match(uri, context));
    }
}
