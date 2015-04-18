#include "parser/filter_pattern.hpp"
#include "pattern/regex_pattern.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

using namespace adblock;
namespace qi = boost::spirit::qi;

using PatternPtr = std::shared_ptr<Pattern>;

const static parser::FilterPattern grammar;

TEST(RegexPatternParser, normal)
{
    const auto &line = boost::as_literal(R"(/.*adblock.*/)");
    std::shared_ptr<Pattern> result;
    const auto rv = qi::parse(line.begin(), line.end(), grammar, result);

    EXPECT_TRUE(rv);
    EXPECT_TRUE(!!std::dynamic_pointer_cast<RegexPattern>(result));
    EXPECT_EQ(".*adblock.*", boost::lexical_cast<std::string>(*result));
}

TEST(RegexPatternParser, noTrailingSlash)
{
    const auto &line = boost::as_literal(R"(/.*adblock.*)");
    std::shared_ptr<Pattern> result;
    const auto rv = qi::parse(line.begin(), line.end(), grammar, result);

    EXPECT_TRUE(rv);
    EXPECT_FALSE(!!std::dynamic_pointer_cast<RegexPattern>(result));
}

TEST(RegexPatternParser, noHeadingSlash)
{
    const auto &line = boost::as_literal(R"(.*adblock.*/)");
    std::shared_ptr<Pattern> result;
    const auto rv = qi::parse(line.begin(), line.end(), grammar, result);

    EXPECT_TRUE(rv);
    EXPECT_FALSE(!!std::dynamic_pointer_cast<RegexPattern>(result));
}

TEST(RegexPatternParser, slashInidePattern)
{
    const auto &line = boost::as_literal(R"(/.*ad/block.*/)");
    std::shared_ptr<Pattern> result;
    const auto rv = qi::parse(line.begin(), line.end(), grammar, result);

    EXPECT_TRUE(rv);
    EXPECT_TRUE(!!std::dynamic_pointer_cast<RegexPattern>(result));
    EXPECT_EQ(".*ad/block.*", boost::lexical_cast<std::string>(*result));
}
