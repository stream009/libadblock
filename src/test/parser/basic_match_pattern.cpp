#include "parser/parser.hpp"

#include "pattern/basic_match_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "type.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_BasicMatchPatternParser, Basic)
{
    auto const& line = "adblock"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& pattern = dynamic_cast<BasicMatchPattern const&>(filter->pattern());

    EXPECT_EQ("adblock", boost::lexical_cast<std::string>(pattern));
    EXPECT_FALSE(pattern.isBeginMatch());
    EXPECT_FALSE(pattern.isEndMatch());
    EXPECT_EQ("adblock"_r, pattern.tokens().front());
}

TEST(Parser_BasicMatchPatternParser, BeginMatch)
{
    auto const& line = "|adblock"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& pattern = dynamic_cast<BasicMatchPattern const&>(filter->pattern());

    EXPECT_EQ("adblock", boost::lexical_cast<std::string>(pattern));
    EXPECT_TRUE(pattern.isBeginMatch());
    EXPECT_FALSE(pattern.isEndMatch());
    EXPECT_EQ("adblock"_r, pattern.tokens().front());
}

TEST(Parser_BasicMatchPatternParser, EndMatch)
{
    auto const& line = "adblock|"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& pattern = dynamic_cast<BasicMatchPattern const&>(filter->pattern());

    EXPECT_EQ("adblock", boost::lexical_cast<std::string>(pattern));
    EXPECT_FALSE(pattern.isBeginMatch());
    EXPECT_TRUE(pattern.isEndMatch());
    EXPECT_EQ("adblock"_r, pattern.tokens().front());
}

TEST(Parser_BasicMatchPatternParser, ExactMatch)
{
    auto const& line = "|adblock|"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& pattern = dynamic_cast<BasicMatchPattern const&>(filter->pattern());

    EXPECT_EQ("adblock", boost::lexical_cast<std::string>(pattern));
    EXPECT_TRUE(pattern.isBeginMatch());
    EXPECT_TRUE(pattern.isEndMatch());
    EXPECT_EQ("adblock"_r, pattern.tokens().front());
}

TEST(Parser_BasicMatchPatternParser, BarInMiddleOfPattern)
{
    auto const& line = "ad|block"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& pattern = dynamic_cast<BasicMatchPattern const&>(filter->pattern());

    EXPECT_EQ("ad|block", boost::lexical_cast<std::string>(pattern));
    EXPECT_FALSE(pattern.isBeginMatch());
    EXPECT_FALSE(pattern.isEndMatch());
    EXPECT_EQ("ad|block"_r, pattern.tokens().front());
}

TEST(Parser_BasicMatchPatternParser, MultiTokenPattern)
{
    auto const& line = "ad*block"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& pattern = dynamic_cast<BasicMatchPattern const&>(filter->pattern());

    EXPECT_EQ("ad*block", boost::lexical_cast<std::string>(pattern));
    EXPECT_FALSE(pattern.isBeginMatch());
    EXPECT_FALSE(pattern.isEndMatch());
    EXPECT_EQ("ad"_r, pattern.tokens().front());
}
