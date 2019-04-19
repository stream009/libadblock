#include "parser/parser.hpp"

#include "pattern/regex_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "core/type.hpp"

#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_RegexPatternParser, normal)
{
    auto const& line = "/.*adblock.*/"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& pattern = dynamic_cast<RegexPattern const&>(filter->pattern());

    EXPECT_EQ(".*adblock.*", boost::lexical_cast<std::string>(pattern));
}

TEST(Parser_RegexPatternParser, noTrailingSlash)
{
    auto const& line = "/.*adblock.*"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    EXPECT_FALSE(dynamic_cast<RegexPattern const*>(&filter->pattern()));
}

TEST(Parser_RegexPatternParser, noHeadingSlash)
{
    auto const& line = ".*adblock.*/"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    EXPECT_FALSE(dynamic_cast<RegexPattern const*>(&filter->pattern()));
}

TEST(Parser_RegexPatternParser, slashInidePattern)
{
    auto const& line = "/.*ad/block.*/"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& pattern = dynamic_cast<RegexPattern const&>(filter->pattern());

    EXPECT_EQ(".*ad/block.*", boost::lexical_cast<std::string>(pattern));
}
