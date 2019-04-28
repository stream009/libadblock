#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "rule/basic_filter_rule.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_DomainMatchPatternParser, Basic)
{
    auto const& line = "||adblock.org"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    auto const& pattern = dynamic_cast<DomainMatchPattern const&>(rule->pattern());

    EXPECT_EQ("adblock.org", boost::lexical_cast<std::string>(pattern));
    EXPECT_FALSE(pattern.isEndMatch());
}

TEST(Parser_DomainMatchPatternParser, WithEndMatch)
{
    auto const& line = "||adblock.org|"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    auto const& pattern = dynamic_cast<DomainMatchPattern const&>(rule->pattern());

    EXPECT_EQ("adblock.org", boost::lexical_cast<std::string>(pattern));
    EXPECT_TRUE(pattern.isEndMatch());
}

TEST(Parser_DomainMatchPatternParser, BarInMiddleOfPattern)
{
    auto const& line = "||adblock.org/foo|bar"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    auto const& pattern = dynamic_cast<DomainMatchPattern const&>(rule->pattern());

    EXPECT_EQ("adblock.org/foo|bar", boost::lexical_cast<std::string>(pattern));
    EXPECT_FALSE(pattern.isEndMatch());
}

TEST(Parser_DomainMatchPatternParser, MultiTokenDomain)
{
    auto const& line = "||ad*block.org/foo"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    auto const& pattern = dynamic_cast<DomainMatchPattern const&>(rule->pattern());

    EXPECT_EQ("ad*block.org/foo", boost::lexical_cast<std::string>(pattern));
    EXPECT_FALSE(pattern.isEndMatch());
}
