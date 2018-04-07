#include "type.hpp"
#include "parser/header.hpp"

#include <gtest/gtest.h>

using namespace adblock;

static const adblock::parser::Header grammar;

enum ParseResult { FullMatch, PartialMatch, NoMatch };

static ParseResult
tryParse(const StringRange &range, StringRange &result)
{
    namespace qi = boost::spirit::qi;

    auto it = range.begin();
    const auto rv = qi::phrase_parse(
                    it, range.end(), grammar, qi::ascii::space, result);
    if (!rv) return ParseResult::NoMatch;
    if (it != range.end()) {
        return ParseResult::PartialMatch;
    }
    return ParseResult::FullMatch;
}

TEST(Header, Elementary)
{
    StringRange version;
    EXPECT_EQ(FullMatch, tryParse("[Adblock Plus      1]"_r, version));
    EXPECT_EQ("1"_r, version);
    EXPECT_EQ(FullMatch, tryParse("[Adblock Plus    2.0]"_r, version));
    EXPECT_EQ("2.0"_r, version);
    EXPECT_EQ(FullMatch, tryParse("[Adblock Plus 1.5.30]"_r, version));
    EXPECT_EQ("1.5.30"_r, version);
}

TEST(Header, WrongFormat)
{
    StringRange version;
    EXPECT_EQ(NoMatch, tryParse("[Adblock 2.0]"_r, version));
    EXPECT_EQ(NoMatch, tryParse("[Adblock Plus 2.0"_r, version));
    EXPECT_EQ(NoMatch, tryParse("Adblock Plus 2.0]"_r, version));
    EXPECT_EQ(NoMatch, tryParse("Adblock Plus 2.0"_r, version));
}

TEST(Header, WrongVersionString)
{
    StringRange version;
    EXPECT_EQ(NoMatch, tryParse("[Adblock Plus 2.0a]"_r, version));
    EXPECT_EQ(NoMatch, tryParse("[Adblock Plus .0  ]"_r, version));
    EXPECT_EQ(NoMatch, tryParse("[Adblock Plus 3.4.]"_r, version));
}
