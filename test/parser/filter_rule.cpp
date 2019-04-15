#include "parser/parser.hpp"

#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/filter_option.hpp"
#include "type.hpp"

#include <memory>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

using namespace adblock;

namespace ba = boost::algorithm;

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

    ASSERT_EQ(0, filter->numOptions());
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

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::MatchCase));
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

    ASSERT_EQ(2, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::ScriptInv));
    EXPECT_TRUE(filter->hasOption(FilterOption::MatchCase));
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

    ASSERT_EQ(2, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::Domain));
    EXPECT_TRUE(filter->hasOption(FilterOption::MatchCase));

    auto* const domains = filter->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_TRUE(ba::equals("foo"_r, (*domains)[0]));
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

    ASSERT_EQ(0, filter->numOptions());
}
