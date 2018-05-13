#include "parser/filter_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

using namespace adblock;
namespace qi = boost::spirit::qi;

const static parser::FilterPattern grammar;

TEST(DomainMatchPatternParser, Basic)
{
    const auto &line = boost::as_literal("||adblock.org");
    std::shared_ptr<Pattern> result;
    const auto rv = qi::parse(line.begin(), line.end(), grammar, result);

    ASSERT_TRUE(rv);
    const auto &pattern = std::dynamic_pointer_cast<DomainMatchPattern>(result);
    EXPECT_TRUE(!!pattern);
    EXPECT_EQ("adblock.org", boost::lexical_cast<std::string>(*pattern));
    EXPECT_FALSE(pattern->isBeginMatch());
    EXPECT_FALSE(pattern->isEndMatch());
    EXPECT_EQ("adblock.org"_r, pattern->tokens().front());
}

TEST(DomainMatchPatternParser, WithEndMatch)
{
    const auto &line = boost::as_literal("||adblock.org|");
    std::shared_ptr<Pattern> result;
    const auto rv = qi::parse(line.begin(), line.end(), grammar, result);

    ASSERT_TRUE(rv);
    const auto &pattern = std::dynamic_pointer_cast<DomainMatchPattern>(result);
    EXPECT_TRUE(!!pattern);
    EXPECT_EQ("adblock.org", boost::lexical_cast<std::string>(*pattern));
    EXPECT_FALSE(pattern->isBeginMatch());
    EXPECT_TRUE(pattern->isEndMatch());
    EXPECT_EQ("adblock.org"_r, pattern->tokens().front());
}

TEST(DomainMatchPatternParser, BarInMiddleOfPattern)
{
    const auto &line = boost::as_literal("||adblock.org/foo|bar");
    std::shared_ptr<Pattern> result;
    const auto rv = qi::parse(line.begin(), line.end(), grammar, result);

    ASSERT_TRUE(rv);
    const auto &pattern = std::dynamic_pointer_cast<DomainMatchPattern>(result);
    EXPECT_TRUE(!!pattern);
    EXPECT_EQ("adblock.org/foo|bar", boost::lexical_cast<std::string>(*pattern));
    EXPECT_FALSE(pattern->isBeginMatch());
    EXPECT_FALSE(pattern->isEndMatch());
    EXPECT_EQ("adblock.org"_r, pattern->tokens().front());
}

TEST(DomainMatchPatternParser, MultiTokenDomain)
{
    const auto &line = boost::as_literal("||ad*block.org/foo");
    std::shared_ptr<Pattern> result;
    const auto rv = qi::parse(line.begin(), line.end(), grammar, result);

    ASSERT_TRUE(rv);
    const auto &pattern = std::dynamic_pointer_cast<DomainMatchPattern>(result);
    EXPECT_TRUE(!!pattern);
    EXPECT_EQ("ad*block.org/foo", boost::lexical_cast<std::string>(*pattern));
    EXPECT_FALSE(pattern->isBeginMatch());
    EXPECT_FALSE(pattern->isEndMatch());
    EXPECT_EQ("ad"_r, pattern->tokens().front());
}
