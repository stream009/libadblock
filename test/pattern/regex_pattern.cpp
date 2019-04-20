#include "pattern/regex_pattern.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"

#include <gtest/gtest.h>

namespace adblock {

TEST(Pattern_RegexPattern, Match)
{
    RegexPattern const pattern { ".*adblock.*"_r };
    auto const& uri = "http://adblock.org"_u;

    EXPECT_TRUE(pattern.match(uri));
}

TEST(Pattern_RegexPattern, MatchWithDifferentCase1)
{
    RegexPattern const pattern { ".*adblock.*"_r };
    auto const& uri = "http://Adblock.Org"_u;

    EXPECT_TRUE(pattern.match(uri));
}

TEST(Pattern_RegexPattern, MatchWithDifferentCase2)
{
    RegexPattern const pattern { ".*AdBlock.*"_r };
    auto const& uri = "http://adblock.org"_u;

    EXPECT_TRUE(pattern.match(uri));
}

TEST(Pattern_RegexPattern, CaseSensitiveMatch)
{
    RegexPattern const pattern { ".*AdBlock.*"_r };
    auto const& uri1 = "http://AdBlock.org"_u;
    auto const& uri2 = "http://adblock.org"_u;

    EXPECT_TRUE(pattern.match(uri1, true));
    EXPECT_TRUE(pattern.match(uri2, false));
}

} // namespace adblock
