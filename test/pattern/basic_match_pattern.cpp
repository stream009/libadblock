#include "pattern/basic_match_pattern.hpp"

#include "type.hpp"

#include <iostream>

#include <boost/format.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>

#include <gtest/gtest.h>

namespace adblock {

TEST(Pattern_BasicMatchPattern, SubstringMatch)
{
    {   // substring match
        BasicMatchPattern pattern { "google"_r };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // match at the begining
        BasicMatchPattern pattern { "http"_r };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // match at the end
        BasicMatchPattern pattern { "com"_r };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // doesn't match
        BasicMatchPattern pattern { "yahoo"_r };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(false, pattern.match(uri));
    }
}

TEST(Pattern_BasicMatchPattern, BeginMatch)
{
    {   // begin match
        BasicMatchPattern pattern { "http://www"_r, true };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // partial match but doesn't match at the begining
        BasicMatchPattern pattern { "google"_r, true };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(false, pattern.match(uri));
    }
    {   // match at the end but doesn't match at the begining
        BasicMatchPattern pattern { "com"_r, true };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(false, pattern.match(uri));
    }
    {   // doesn't match at anywhere
        BasicMatchPattern pattern { "yahoo"_r, true };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(false, pattern.match(uri));
    }
}

TEST(Pattern_BasicMatchPattern, EndMatch)
{
    {   // match at the ending
        BasicMatchPattern pattern { ".com"_r, false, true };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // substring match but doesn't match at the end
        BasicMatchPattern pattern { "google"_r, false, true };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(false, pattern.match(uri));
    }
    {   // match at the begining but doesn't match at the end
        BasicMatchPattern pattern { "http"_r, false, true };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(false, pattern.match(uri));
    }
    {   // doesn't match anywhere
        BasicMatchPattern pattern { "yahoo"_r, false, true };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(false, pattern.match(uri));
    }
}

TEST(Pattern_BasicMatchPattern, ExactMatch)
{
    {   // exact match
        BasicMatchPattern pattern {
                     "http://www.google.com"_r, true, true };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // match at begining but ending doesn't match
        BasicMatchPattern pattern {
                    "http://www.google"_r, true, true };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(false, pattern.match(uri));
    }
    {   // match at the end but begining doesn't match
        BasicMatchPattern pattern {
                        "www.google.com"_r, true, true };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(false, pattern.match(uri));
    }
    {   // match at the begining but ending doesn't match
        BasicMatchPattern pattern {
                "http://www.google.com/test"_r, true, true };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(false, pattern.match(uri));
    }
    {   // match at the begining and the end but middle part doesn't match
        BasicMatchPattern pattern {
                     "http://www.yahoo.com"_r, true, true };
        Uri uri { "http://www.google.com" };

        EXPECT_EQ(false, pattern.match(uri));
    }
}

TEST(Pattern_BasicMatchPattern, WildcardMatch)
{
    {   // single asterisk at middle of the pattern
        BasicMatchPattern pattern {
            "www*com"_r
        };
        Uri uri { "http://www.goole.com" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // two asterisks at middle of the pattern
        BasicMatchPattern pattern {
            "www*com*jpg"_r
        };
        Uri uri { "http://www.goole.com/image/top.jpg" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // one asterisk at left of the pattern (right asterisk is implied)
        BasicMatchPattern pattern {
            "*com"_r
        };
        Uri uri { "http://www.goole.com/image/top.jpg" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // one asterisk at right of the pattern (left asterisk is implied)
        BasicMatchPattern pattern {
            "www*"_r
        };
        Uri uri { "http://www.goole.com/image/top.jpg" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // two asterisks at both end of the pattern
        BasicMatchPattern pattern {
            "*www*"_r
        };
        Uri uri { "http://www.goole.com/image/top.jpg" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // two consecutive asterisks at middle of the pattern
        BasicMatchPattern pattern {
            "www**com"_r
        };
        Uri uri { "http://www.goole.com/image/top.jpg" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // three consecutive asterisks at middle of the pattern
        BasicMatchPattern pattern {
            "www***com"_r
        };
        Uri uri { "http://www.goole.com/image/top.jpg" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // two consecutive asterisks at front of the pattern
        BasicMatchPattern pattern {
            "**www"_r
        };
        Uri uri { "http://www.goole.com/image/top.jpg" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // two consecutive asterisks at back of the pattern
        BasicMatchPattern pattern {
            "www**"_r
        };
        Uri uri { "http://www.goole.com/image/top.jpg" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // two consecutive asterisks at both end of the pattern
        BasicMatchPattern pattern {
            "**www**"_r
        };
        Uri uri { "http://www.goole.com/image/top.jpg" };

        EXPECT_EQ(true, pattern.match(uri));
    }
    {   // it doesn't care about asterisks in the URI
        BasicMatchPattern pattern {
            "image*jpg"_r
        };
        Uri uri { "http://www.google.com/imag*/top.j*g" };

        EXPECT_EQ(false, pattern.match(uri));
    }
}

TEST(Pattern_BasicMatchPattern, Separator)
{
    BasicMatchPattern pattern {
        "image^"_r
    };

    using CharacterRange =
        boost::iterator_range<std::string::const_iterator>;

    auto isSeparator =
        [&](CharacterRange const list, bool const expected)
        {
            for (auto c: list) {
                Uri uri {
                    str(boost::format {
                        "http://www.google.com/image%ctop.jpg"
                    } % c)
                };
                EXPECT_EQ(expected, pattern.match(uri)) << "[" << c << "]";
            }
        };

    // doesn't match with alphabets, digits and other characters
    // defined in the specification.
    std::string notSeparator =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "_~.%"
    ;

    std::string uriCharacters =
        /* ALPHA       */ "abcdefghijklmnopqrstuvwxyz"
                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        /* DIGIT       */ "0123456789"
        /* unreserved  */ "-._~"
        /* gen-delims  */ ":/?#[]@"
        /* sub-delims  */ "!$&'()*+,;="
        /* pct-encoded */ "%"
    ;

    // check test data's validity
    boost::sort(uriCharacters);
    boost::sort(notSeparator);
    ASSERT_TRUE(boost::includes(uriCharacters, notSeparator));

    // extract separator set from test data
    std::string separator;
    boost::set_difference(uriCharacters, notSeparator,
        std::back_inserter(separator)
    );

    isSeparator(separator, true);
    isSeparator(notSeparator, false);
}

TEST(Pattern_BasicMatchPattern, EndOfLineAsSeparator)
{
    {
        BasicMatchPattern pattern { "com^"_r };
        Uri uri { "http://www.google.com/" };

        EXPECT_TRUE(pattern.match(uri));
    }
    {
        BasicMatchPattern pattern { "com^"_r };
        Uri uri { "http://www.google.com" };

        EXPECT_TRUE(pattern.match(uri));
    }
}

TEST(Pattern_BasicMatchPattern, MatchShouldBeCaseInsensitive1)
{
    BasicMatchPattern pattern { ".jpg"_r };
    auto const& uri = "http://www.adblock.org/SomeThing.Jpg"_u;
    EXPECT_TRUE(pattern.match(uri));
}

TEST(Pattern_BasicMatchPattern, MatchShouldBeCaseInsensitive2)
{
    BasicMatchPattern pattern { ".Jpg"_r };
    auto const& uri = "http://www.adblock.org/something.jpg"_u;
    EXPECT_TRUE(pattern.match(uri));
}

TEST(Pattern_BasicMatchPattern, CaseSensitiveMatch)
{
    BasicMatchPattern pattern { ".Jpg"_r };
    auto const& uri = "http://www.adblock.org/SomeThing.Jpg"_u;
    EXPECT_TRUE(pattern.match(uri, true));
}

TEST(Pattern_BasicMatchPattern, CaseSensitiveUnmatch)
{
    BasicMatchPattern pattern { ".jpg"_r };
    auto const& uri = "http://www.adblock.org/SomeThing.Jpg"_u;
    EXPECT_FALSE(pattern.match(uri, true));
}

TEST(Pattern_BasicMatchPattern, InvalidUseOfAnchor1)
{
    BasicMatchPattern pattern { "*foo"_r, true };
    EXPECT_TRUE(!pattern.isBeginMatch());
}

TEST(Pattern_BasicMatchPattern, InvalidUseOfAnchor2)
{
    BasicMatchPattern pattern { "*foo*"_r, false, true };
    EXPECT_TRUE(!pattern.isEndMatch());
}

TEST(Pattern_BasicMatchPattern, InvalidUseOfAnchor3)
{
    BasicMatchPattern pattern { "*foo*"_r, true, true };
    EXPECT_TRUE(!pattern.isBeginMatch());
    EXPECT_TRUE(!pattern.isEndMatch());
}

} // namespace adblock
