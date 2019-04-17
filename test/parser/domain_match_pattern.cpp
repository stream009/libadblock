#include "parser/parser.hpp"

#include "pattern/domain_match_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "type.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_DomainMatchPatternParser, Basic)
{
    auto const& line = "||adblock.org"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& pattern = dynamic_cast<DomainMatchPattern const&>(filter->pattern());

    EXPECT_EQ("adblock.org", boost::lexical_cast<std::string>(pattern));
    EXPECT_FALSE(pattern.isEndMatch());
    EXPECT_EQ("adblock.org"_r, pattern.tokens().front());
}

TEST(Parser_DomainMatchPatternParser, WithEndMatch)
{
    auto const& line = "||adblock.org|"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& pattern = dynamic_cast<DomainMatchPattern const&>(filter->pattern());

    EXPECT_EQ("adblock.org", boost::lexical_cast<std::string>(pattern));
    EXPECT_TRUE(pattern.isEndMatch());
    EXPECT_EQ("adblock.org"_r, pattern.tokens().front());
}

TEST(Parser_DomainMatchPatternParser, BarInMiddleOfPattern)
{
    auto const& line = "||adblock.org/foo|bar"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& pattern = dynamic_cast<DomainMatchPattern const&>(filter->pattern());

    EXPECT_EQ("adblock.org/foo|bar", boost::lexical_cast<std::string>(pattern));
    EXPECT_FALSE(pattern.isEndMatch());
    EXPECT_EQ("adblock.org"_r, pattern.tokens().front());
}

TEST(Parser_DomainMatchPatternParser, MultiTokenDomain)
{
    auto const& line = "||ad*block.org/foo"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& pattern = dynamic_cast<DomainMatchPattern const&>(filter->pattern());

    EXPECT_EQ("ad*block.org/foo", boost::lexical_cast<std::string>(pattern));
    EXPECT_FALSE(pattern.isEndMatch());
    EXPECT_EQ("ad"_r, pattern.tokens().front());
}
