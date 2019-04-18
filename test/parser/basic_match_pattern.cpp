#include "parser/parser.hpp"

#include "pattern/basic_match_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "type.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_BasicMatchPattern, Basic)
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
}

TEST(Parser_BasicMatchPattern, BeginMatch)
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
}

TEST(Parser_BasicMatchPattern, EndMatch)
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
}

TEST(Parser_BasicMatchPattern, ExactMatch)
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
}

TEST(Parser_BasicMatchPattern, BarInMiddleOfPattern)
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
}

TEST(Parser_BasicMatchPattern, MultiTokenPattern)
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
}
