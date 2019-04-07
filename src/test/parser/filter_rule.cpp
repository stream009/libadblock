#include "parser/parser.hpp"

#include "option.hpp"
#include "option/domain_option.hpp"
#include "option/match_case_option.hpp"
#include "option/script_option.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "type.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_BasicFilterRule, Basic)
{
    auto const& line = "adblock.org"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<BasicFilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto* const pattern =
        dynamic_cast<BasicMatchPattern const*>(&filter->pattern());
    ASSERT_TRUE(pattern);

    EXPECT_TRUE(filter->options().empty());
}

TEST(Parser_BasicFilterRule, OneOption)
{
    auto const& line = "||adblock.org$match-case"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<BasicFilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto* const pattern =
        dynamic_cast<DomainMatchPattern const*>(&filter->pattern());
    ASSERT_TRUE(pattern);

    auto const& options = filter->options();
    ASSERT_EQ(1, options.size());
    EXPECT_TRUE(dynamic_cast<MatchCaseOption*>(options[0].get()));
}

TEST(Parser_BasicFilterRule, TwoOptions)
{
    auto const& line = "adblock.org$~script,match-case"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<BasicFilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto* const pattern =
        dynamic_cast<BasicMatchPattern const*>(&filter->pattern());
    ASSERT_TRUE(pattern);

    auto const& options = filter->options();
    ASSERT_EQ(2, options.size());

    auto* const scriptOption = dynamic_cast<ScriptOption*>(options[0].get());
    ASSERT_TRUE(scriptOption);
    EXPECT_TRUE(scriptOption->inverse());

    EXPECT_TRUE(dynamic_cast<MatchCaseOption*>(options[1].get()));
}

TEST(Parser_BasicFilterRule, EmptyPatternWithOption)
{
    auto const& line = "$domain=foo,match-case"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<BasicFilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto* const pattern =
        dynamic_cast<BasicMatchPattern const*>(&filter->pattern());
    ASSERT_TRUE(pattern);
    EXPECT_EQ(""_r, pattern->pattern());

    auto const& options = filter->options();
    EXPECT_EQ(2, options.size());

    EXPECT_TRUE(dynamic_cast<DomainOption*>(options[0].get()));
    EXPECT_TRUE(dynamic_cast<MatchCaseOption*>(options[1].get()));
}

TEST(Parser_ExceptionFilterRule, Basic)
{
    auto const& line = "@@adblock.org"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<ExceptionFilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto* const pattern =
        dynamic_cast<BasicMatchPattern const*>(&filter->pattern());
    ASSERT_TRUE(pattern);
    EXPECT_EQ("adblock.org", boost::lexical_cast<std::string>(*pattern));

    EXPECT_TRUE(filter->options().empty());
}
