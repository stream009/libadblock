#include "type.hpp"
#include "option.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "pattern/basic_match_pattern.hpp"

#include "../mock_context.hpp"

#include <memory>

#include <boost/range/iterator_range.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(BasicFilterRule, Basic)
{
    const auto &pattern =
        std::make_shared<BasicMatchPattern>("Adblock"_r);
    BasicFilterRule rule { pattern, boost::none };

    EXPECT_EQ(&(*pattern), &(rule.pattern()));
    EXPECT_TRUE(rule.options().empty());

    const MockContext context {};
    const auto &uri = "http://adblock.org"_u;
    EXPECT_TRUE(rule.match(uri, &context));
}

TEST(BasicFilterRule, WithOption)
{
    const auto &pattern =
        std::make_shared<BasicMatchPattern>("adblock"_r);
    FilterRule::Options theOptions;
    theOptions.push_back(std::make_shared<ScriptOption>(false));

    BasicFilterRule rule { pattern, theOptions };

    EXPECT_EQ(&(*pattern), &(rule.pattern()));
    const auto &options = rule.options();
    ASSERT_EQ(1, options.size());
    const auto &option = std::dynamic_pointer_cast<ScriptOption>(options[0]);
    ASSERT_TRUE(!!option);
    EXPECT_EQ(theOptions[0], option);

    struct ScriptContext : MockContext {
        bool fromScriptTag() const { return true; }
    } context {};

    const auto &uri = "http://adblock.org"_u;
    EXPECT_TRUE(rule.match(uri, &context));
}

TEST(BasicFilterRule, CaseSensitiveMatch)
{
    const auto &pattern =
        std::make_shared<BasicMatchPattern>("Adblock"_r);
    FilterRule::Options theOptions;
    theOptions.push_back(std::make_shared<MatchCaseOption>());

    const BasicFilterRule rule { pattern, theOptions };
    const MockContext context {};
    {
        const auto &uri = "http://adblock.org"_u;
        EXPECT_FALSE(rule.match(uri, &context));
    }
    {
        const auto &uri = "http://Adblock.org"_u;
        EXPECT_TRUE(rule.match(uri, &context));
    }
}

TEST(ExceptionFilterRule, Basic)
{
    const auto &pattern =
        std::make_shared<BasicMatchPattern>("Adblock"_r);
    const ExceptionFilterRule rule { pattern, boost::none };

    EXPECT_EQ(&(*pattern), &(rule.pattern()));
    EXPECT_TRUE(rule.options().empty());

    const MockContext context {};
    const auto &uri = "http://adblock.org"_u;
    EXPECT_TRUE(rule.match(uri, &context));
}

TEST(ExceptionFilterRule, WithOption)
{
    const auto &pattern =
        std::make_shared<BasicMatchPattern>("adblock"_r);
    FilterRule::Options theOptions;
    theOptions.push_back(std::make_shared<ScriptOption>(false));

    const ExceptionFilterRule rule { pattern, theOptions };

    EXPECT_EQ(&(*pattern), &(rule.pattern()));
    const auto &options = rule.options();
    ASSERT_EQ(1, options.size());
    const auto &option = std::dynamic_pointer_cast<ScriptOption>(options[0]);
    ASSERT_TRUE(!!option);
    EXPECT_EQ(theOptions[0], option);

    struct ScriptContext : MockContext {
        bool fromScriptTag() const { return true; }
    } context {};

    const auto &uri = "http://adblock.org"_u;
    EXPECT_TRUE(rule.match(uri, &context));
}

TEST(ExceptionFilterRule, CaseSensitiveMatch)
{
    const auto &pattern =
        std::make_shared<BasicMatchPattern>("Adblock"_r);
    FilterRule::Options theOptions;
    theOptions.push_back(std::make_shared<MatchCaseOption>());

    const ExceptionFilterRule rule { pattern, theOptions };
    const MockContext context {};
    {
        const auto &uri = "http://adblock.org"_u;
        EXPECT_FALSE(rule.match(uri, &context));
    }
    {
        const auto &uri = "http://Adblock.org"_u;
        EXPECT_TRUE(rule.match(uri, &context));
    }
}

