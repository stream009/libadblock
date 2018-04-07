#include "type.hpp"
#include "parser/parser.hpp"

#include <gtest/gtest.h>

using namespace adblock;

TEST(ParseHeader, Elementary)
{
    StringRange result;
    EXPECT_EQ("1"_r,      parser::parseHeader("[Adblock Plus      1]"_r));
    EXPECT_EQ("2.0"_r,    parser::parseHeader("[Adblock Plus    2.0]"_r));
    EXPECT_EQ("1.5.30"_r, parser::parseHeader("[Adblock Plus 1.5.30]"_r));
}

TEST(ParseHeader, FailToParse)
{
    StringRange result;
    EXPECT_EQ(""_r, parser::parseHeader("[Adblock 1.0]"_r));
    EXPECT_EQ(""_r, parser::parseHeader("Adblock Plus 2.0]"_r));
}

TEST(ParseHeader, PartialMatch)
{
    StringRange result;
    EXPECT_EQ("1.5.30"_r, parser::parseHeader("[Adblock Plus 1.5.30"_r));
}
