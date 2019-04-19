#include "pattern/domain_match_pattern.hpp"
#include "core/type.hpp"

#include <gtest/gtest.h>

namespace adblock {

TEST(Pattern_DomainMatchPatternTest, standard)
{
    {   // match
        DomainMatchPattern pattern {
            "google.com/foo"_r, false
        };

        Uri url { "http://www.google.com/foo" };

        EXPECT_EQ(true, pattern.match(url));
    }
    {   // domain part doesn't match
        DomainMatchPattern pattern {
            "google.com/foo"_r, false
        };

        Uri url { "http://www.google.net/foo" };

        EXPECT_EQ(false, pattern.match(url));
    }
    {   // after domain part doesn't match
        DomainMatchPattern pattern {
            "google.com/foo"_r, false
        };

        Uri url { "http://www.google.com/bar" };

        EXPECT_EQ(false, pattern.match(url));
    }
    {   // both part doesn't match
        DomainMatchPattern pattern {
            "google.com/foo"_r, false
        };

        Uri url { "http://www.google.net/bar" };

        EXPECT_EQ(false, pattern.match(url));
    }
}

TEST(Pattern_DomainMatchPatternTest, withEndMatch)
{
    {   // match
        DomainMatchPattern pattern {
            "google.com/foo"_r, true
        };

        Uri url { "http://www.google.com/foo" };

        EXPECT_EQ(true, pattern.match(url));
    }
    {   // domain part doesn't match
        DomainMatchPattern pattern {
            "google.com/foo"_r, true
        };

        Uri url { "http://www.google.net/foo" };

        EXPECT_EQ(false, pattern.match(url));
    }
    {   // after domain part doesn't match
        DomainMatchPattern pattern {
            "google.com/foo"_r, true
        };

        Uri url { "http://www.google.com/bar" };

        EXPECT_EQ(false, pattern.match(url));
    }
    {   // after domain part match partially but end doesn't match
        DomainMatchPattern pattern {
            "google.com/foo"_r, true
        };

        Uri url { "http://www.google.com/fooz" };

        EXPECT_EQ(false, pattern.match(url));
    }
    {   // both part doesn't match
        DomainMatchPattern pattern {
            "google.com/foo"_r, true
        };

        Uri url { "http://www.google.net/foo" };

        EXPECT_EQ(false, pattern.match(url));
    }
}

TEST(Pattern_DomainMatchPatternTest, wildCard)
{
    {   // asterisk in the domain part
        DomainMatchPattern pattern {
            "www.goo*.com/foo"_r, false
        };

        Uri url { "http://www.google.com/foo" };

        EXPECT_EQ(true, pattern.match(url));
    }
    {   // asterisk in after the domain part
        DomainMatchPattern pattern {
            "www.google.com/*o"_r, false
        };

        Uri url { "http://www.google.com/foo" };

        EXPECT_EQ(true, pattern.match(url));
    }
    {   // asterisk in the both part
        DomainMatchPattern pattern {
            "www.go*le.com/*o"_r, false
        };

        Uri url { "http://www.google.com/foo" };

        EXPECT_EQ(true, pattern.match(url));
    }
}

TEST(Pattern_DomainMatchPatternTest, separator)
{
    {   // separator at right after the domain part
        DomainMatchPattern pattern {
            "www.google.com^mail"_r, false
        };

        Uri url { "http://www.google.com/mail" };

        EXPECT_TRUE(pattern.match(url));
    }
    {   // separator at middle of after the domain part
        DomainMatchPattern pattern {
            "www.google.com/mail^foo"_r, false
        };

        Uri url { "http://www.google.com/mail?foo" };

        EXPECT_TRUE(pattern.match(url));
    }
    {   // separator at end of after the domain part
        DomainMatchPattern pattern {
            "www.google.com/mail^"_r, false
        };

        Uri url { "http://www.google.com/mail/" };

        EXPECT_TRUE(pattern.match(url));
    }
    {   // separator at end of after the domain part
        // and it has to match with end of the URL
        DomainMatchPattern pattern {
            "www.google.com/mail^"_r, false
        };

        Uri url { "http://www.google.com/mail" };

        EXPECT_TRUE(pattern.match(url));
    }
}

TEST(Pattern_DomainMatchPatternTest, DomainOnly)
{
    DomainMatchPattern pattern {
        "www.google.com"_r, false
    };

    Uri url { "http://www.google.com/mail" };

    EXPECT_TRUE(pattern.match(url));
}

TEST(Pattern_DomainMatchPatternTest, PathComponentMustMatchFromBeginning)
{
    DomainMatchPattern pattern {
        "openload.co/r"_r, false
    };

    Uri url { "https://openload.co/f/R" };

    EXPECT_FALSE(pattern.match(url));
}

TEST(Pattern_DomainMatchPattern, InvalidUseOfAnchor)
{
    DomainMatchPattern pattern { "foo*"_r, true };
    EXPECT_TRUE(!pattern.isEndMatch());
}

TEST(Pattern_DomainMatchPattern, BugFix1)
{
    DomainMatchPattern pattern { "example.com/image.js"_r };

    {
        Uri url { "http://www.example.com/image.js" };
        EXPECT_TRUE(pattern.match(url));
    }
    {
        Uri url { "http://www.example.com.au/image.js" };
        EXPECT_FALSE(pattern.match(url));
    }
}

TEST(Pattern_DomainMatchPattern, DomainThatStartsWithWildcard)
{
    DomainMatchPattern pattern { "*.com/image.js"_r };

    {
        Uri url { "http://www.example.com/image.js" };
        EXPECT_TRUE(pattern.match(url));
    }
}

TEST(Pattern_DomainMatchPattern, DomainThatEndsWithWildcard)
{
    DomainMatchPattern pattern { "example.*/image.js"_r };

    {
        Uri url { "http://www.example.com/image.js" };
        EXPECT_TRUE(pattern.match(url));
    }
    {
        Uri url { "http://www.example.net/image.js" };
        EXPECT_TRUE(pattern.match(url));
    }
}

TEST(Pattern_DomainMatchPattern, DomainPatternMustMatchWithDotBoundary)
{
    DomainMatchPattern pattern { "example.com/image.js"_r };

    {
        Uri url { "http://www.example.com/image.js" };
        EXPECT_TRUE(pattern.match(url));
    }
    {
        Uri url { "http://www.someexample.com/image.js" };
        EXPECT_FALSE(pattern.match(url));
    }
}

TEST(Pattern_DomainMatchPattern, PathEndsWithWildcard)
{
    DomainMatchPattern pattern { "reddit.com^*_sponsor.png?"_r };

    {
        Uri url { "https://www.reddit.com/r/all" };
        EXPECT_FALSE(pattern.match(url));
    }
    {
        Uri url { "https://www.reddit.com/all/an_sponsor.png?" };
        EXPECT_TRUE(pattern.match(url));
    }
}

} // namespace adblock
