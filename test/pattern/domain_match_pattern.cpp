#include "pattern/domain_match_pattern.hpp"

#include <boost/range/iterator_range.hpp>
#include <gtest/gtest.h>

namespace adblock {

TEST(DomainMatchPatternTest, standard)
{
    {   // match
        DomainMatchPattern pattern {
            boost::as_literal("google.com"),
            boost::as_literal("/foo"),
            false
        };

        Uri url { "http://www.google.com/foo" };

        EXPECT_EQ(true, pattern.match(url));
    }
    {   // domain part doesn't match
        DomainMatchPattern pattern {
            boost::as_literal("google.com"),
            boost::as_literal("/foo"),
            false
        };

        Uri url { "http://www.google.net/foo" };

        EXPECT_EQ(false, pattern.match(url));
    }
    {   // after domain part doesn't match
        DomainMatchPattern pattern {
            boost::as_literal("google.com"),
            boost::as_literal("/foo"),
            false
        };

        Uri url { "http://www.google.com/bar" };

        EXPECT_EQ(false, pattern.match(url));
    }
    {   // both part doesn't match
        DomainMatchPattern pattern {
            boost::as_literal("google.com"),
            boost::as_literal("/foo"),
            false
        };

        Uri url { "http://www.google.net/bar" };

        EXPECT_EQ(false, pattern.match(url));
    }
}

TEST(DomainMatchPatternTest, withEndMatch)
{
    {   // match
        DomainMatchPattern pattern {
            boost::as_literal("google.com"),
            boost::as_literal("/foo"),
            true
        };

        Uri url { "http://www.google.com/foo" };

        EXPECT_EQ(true, pattern.match(url));
    }
    {   // domain part doesn't match
        DomainMatchPattern pattern {
            boost::as_literal("google.com"),
            boost::as_literal("/foo"),
            true
        };

        Uri url { "http://www.google.net/foo" };

        EXPECT_EQ(false, pattern.match(url));
    }
    {   // after domain part doesn't match
        DomainMatchPattern pattern {
            boost::as_literal("google.com"),
            boost::as_literal("/foo"),
            true
        };

        Uri url { "http://www.google.com/bar" };

        EXPECT_EQ(false, pattern.match(url));
    }
    {   // after domain part match partially but end doesn't match
        DomainMatchPattern pattern {
            boost::as_literal("google.com"),
            boost::as_literal("/foo"),
            true
        };

        Uri url { "http://www.google.com/fooz" };

        EXPECT_EQ(false, pattern.match(url));
    }
    {   // both part doesn't match
        DomainMatchPattern pattern {
            boost::as_literal("google.com"),
            boost::as_literal("/foo"),
            true
        };

        Uri url { "http://www.google.net/foo" };

        EXPECT_EQ(false, pattern.match(url));
    }
}

TEST(DomainMatchPatternTest, wildCard)
{
    {   // asterisk in the domain part
        DomainMatchPattern pattern {
            boost::as_literal("www.goo*.com"),
            boost::as_literal("/foo"),
            false
        };

        Uri url { "http://www.google.com/foo" };

        EXPECT_EQ(true, pattern.match(url));
    }
    {   // asterisk in after the domain part
        DomainMatchPattern pattern {
            boost::as_literal("www.google.com"),
            boost::as_literal("/*o"),
            false
        };

        Uri url { "http://www.google.com/foo" };

        EXPECT_EQ(true, pattern.match(url));
    }
    {   // asterisk in the both part
        DomainMatchPattern pattern {
            boost::as_literal("www.go*le.com"),
            boost::as_literal("/*o"),
            false
        };

        Uri url { "http://www.google.com/foo" };

        EXPECT_EQ(true, pattern.match(url));
    }
}

TEST(DomainMatchPatternTest, separator)
{
    {   // separator at right after the domain part
        DomainMatchPattern pattern {
            boost::as_literal("www.google.com"),
            boost::as_literal("^mail"),
            false
        };

        Uri url { "http://www.google.com/mail" };

        EXPECT_TRUE(pattern.match(url));
    }
    {   // separator at middle of after the domain part
        DomainMatchPattern pattern {
            boost::as_literal("www.google.com"),
            boost::as_literal("mail^foo"),
            false
        };

        Uri url { "http://www.google.com/mail?foo" };

        EXPECT_TRUE(pattern.match(url));
    }
    {   // separator at end of after the domain part
        DomainMatchPattern pattern {
            boost::as_literal("www.google.com"),
            boost::as_literal("mail^"),
            false
        };

        Uri url { "http://www.google.com/mail/" };

        EXPECT_TRUE(pattern.match(url));
    }
    {   // separator at end of after the domain part
        // and it has to match with end of the URL
        DomainMatchPattern pattern {
            boost::as_literal("www.google.com"),
            boost::as_literal("mail^"),
            false
        };

        Uri url { "http://www.google.com/mail" };

        EXPECT_TRUE(pattern.match(url));
    }
}

TEST(DomainMatchPatternTest, DomainOnly)
{
    DomainMatchPattern pattern {
        boost::as_literal("www.google.com"),
        boost::none,
        false
    };

    Uri url { "http://www.google.com/mail" };

    EXPECT_TRUE(pattern.match(url));
}

} // namespace adblock
