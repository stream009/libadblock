#include "parser/filter_pattern.hpp"
#include "pattern/basic_match_pattern.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

using namespace adblock;
namespace qi = boost::spirit::qi;

const static parser::FilterPattern grammar;

TEST(BasicMatchPatternParser, Basic)
{
    const auto &line = boost::as_literal("adblock");
    std::shared_ptr<Pattern> result;
    const auto rv = qi::parse(line.begin(), line.end(), grammar, result);

    ASSERT_TRUE(rv);
    const auto &pattern = std::dynamic_pointer_cast<BasicMatchPattern>(result);
    EXPECT_TRUE(!!pattern);
    EXPECT_EQ("adblock", boost::lexical_cast<std::string>(*pattern));
    EXPECT_FALSE(pattern->isBeginMatch());
    EXPECT_FALSE(pattern->isEndMatch());
}

TEST(BasicMatchPatternParser, BeginMatch)
{
    const auto &line = boost::as_literal("|adblock");
    std::shared_ptr<Pattern> result;
    const auto rv = qi::parse(line.begin(), line.end(), grammar, result);

    ASSERT_TRUE(rv);
    const auto &pattern = std::dynamic_pointer_cast<BasicMatchPattern>(result);
    EXPECT_TRUE(!!pattern);
    EXPECT_EQ("adblock", boost::lexical_cast<std::string>(*pattern));
    EXPECT_TRUE(pattern->isBeginMatch());
    EXPECT_FALSE(pattern->isEndMatch());
}

TEST(BasicMatchPatternParser, EndMatch)
{
    const auto &line = boost::as_literal("adblock|");
    std::shared_ptr<Pattern> result;
    const auto rv = qi::parse(line.begin(), line.end(), grammar, result);

    ASSERT_TRUE(rv);
    const auto &pattern = std::dynamic_pointer_cast<BasicMatchPattern>(result);
    EXPECT_TRUE(!!pattern);
    EXPECT_EQ("adblock", boost::lexical_cast<std::string>(*pattern));
    EXPECT_FALSE(pattern->isBeginMatch());
    EXPECT_TRUE(pattern->isEndMatch());
}

TEST(BasicMatchPatternParser, ExactMatch)
{
    const auto &line = boost::as_literal("|adblock|");
    std::shared_ptr<Pattern> result;
    const auto rv = qi::parse(line.begin(), line.end(), grammar, result);

    ASSERT_TRUE(rv);
    const auto &pattern = std::dynamic_pointer_cast<BasicMatchPattern>(result);
    EXPECT_TRUE(!!pattern);
    EXPECT_EQ("adblock", boost::lexical_cast<std::string>(*pattern));
    EXPECT_TRUE(pattern->isBeginMatch());
    EXPECT_TRUE(pattern->isEndMatch());
}

TEST(BasicMatchPatternParser, BarInMiddleOfPattern)
{
    const auto &line = boost::as_literal("ad|block");
    std::shared_ptr<Pattern> result;
    const auto rv = qi::parse(line.begin(), line.end(), grammar, result);

    ASSERT_TRUE(rv);
    const auto &pattern = std::dynamic_pointer_cast<BasicMatchPattern>(result);
    EXPECT_TRUE(!!pattern);
    EXPECT_EQ("ad|block", boost::lexical_cast<std::string>(*pattern));
    EXPECT_FALSE(pattern->isBeginMatch());
    EXPECT_FALSE(pattern->isEndMatch());
}
