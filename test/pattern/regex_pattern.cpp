#include "pattern/regex_pattern.hpp"

#include <iostream>

#include <boost/format.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>

#include <gtest/gtest.h>

namespace adblock {

TEST(RegexPattern, Match)
{
    const RegexPattern pattern { ".*adblock.*"_r };
    const auto &uri = "http://adblock.org"_u;

    EXPECT_TRUE(pattern.match(uri));
}

TEST(RegexPattern, MatchWithDifferentCase1)
{
    const RegexPattern pattern { ".*adblock.*"_r };
    const auto &uri = "http://Adblock.Org"_u;

    EXPECT_TRUE(pattern.match(uri));
}

TEST(RegexPattern, MatchWithDifferentCase2)
{
    const RegexPattern pattern { ".*AdBlock.*"_r };
    const auto &uri = "http://adblock.org"_u;

    EXPECT_TRUE(pattern.match(uri));
}

TEST(RegexPattern, CaseSensitiveMatch)
{
    const RegexPattern pattern { ".*AdBlock.*"_r };
    const auto &uri1 = "http://AdBlock.org"_u;
    const auto &uri2 = "http://adblock.org"_u;

    EXPECT_TRUE(pattern.match(uri1, true));
    EXPECT_TRUE(pattern.match(uri2, false));
}

} // namespace adblock
