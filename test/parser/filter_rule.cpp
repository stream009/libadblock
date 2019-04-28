#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/filter_option.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_BasicFilterRule, Basic)
{
    auto const line = "adblock.org"_r;

    auto const rule = parse_rule<BasicFilterRule>(line);
    ASSERT_TRUE(rule);

    auto* const pattern =
        dynamic_cast<BasicMatchPattern const*>(&rule->pattern());
    ASSERT_TRUE(pattern);

    ASSERT_EQ(0, rule->numOptions());
}

TEST(Parser_BasicFilterRule, OneOption)
{
    auto const line = "||adblock.org$match-case"_r;

    auto const rule = parse_rule<BasicFilterRule>(line);
    ASSERT_TRUE(rule);

    auto* const pattern =
        dynamic_cast<DomainMatchPattern const*>(&rule->pattern());
    ASSERT_TRUE(pattern);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::MatchCase));
}

TEST(Parser_BasicFilterRule, TwoOptions)
{
    auto const line = "adblock.org$~script,match-case"_r;

    auto const rule = parse_rule<BasicFilterRule>(line);
    ASSERT_TRUE(rule);

    auto* const pattern =
        dynamic_cast<BasicMatchPattern const*>(&rule->pattern());
    ASSERT_TRUE(pattern);

    ASSERT_EQ(2, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::ScriptInv));
    EXPECT_TRUE(rule->hasOption(FilterOption::MatchCase));
}

TEST(Parser_BasicFilterRule, EmptyPatternWithOption)
{
    auto const line = "$domain=foo,match-case"_r;

    auto const rule = parse_rule<BasicFilterRule>(line);
    ASSERT_TRUE(rule);

    auto* const pattern =
        dynamic_cast<BasicMatchPattern const*>(&rule->pattern());
    ASSERT_TRUE(pattern);
    EXPECT_EQ(""_r, pattern->pattern());

    ASSERT_EQ(2, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::Domain));
    EXPECT_TRUE(rule->hasOption(FilterOption::MatchCase));

    auto* const domains = rule->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_EQ("foo"_r, (*domains)[0]);
}

TEST(Parser_ExceptionFilterRule, Basic)
{
    auto const line = "@@adblock.org"_r;

    auto const rule = parse_rule<ExceptionFilterRule>(line);
    ASSERT_TRUE(rule);

    auto* const pattern =
        dynamic_cast<BasicMatchPattern const*>(&rule->pattern());
    ASSERT_TRUE(pattern);
    EXPECT_EQ("adblock.org", boost::lexical_cast<std::string>(*pattern));

    ASSERT_EQ(0, rule->numOptions());
}
