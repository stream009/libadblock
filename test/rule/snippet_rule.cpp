#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/snippet_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Rule_SnippetRule, NoDomain)
{
    auto const line = "#$#log 'hello'";

    auto const rule = parse_rule<SnippetRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_TRUE(rule->match("http://example.com"_u));
}

TEST(Rule_SnippetRule, SingleDomain_Positive)
{
    auto const line = "example1.com#$#log 'hello'";

    auto const rule = parse_rule<SnippetRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_TRUE(rule->match("http://example1.com"_u));
    EXPECT_FALSE(rule->match("http://example2.com"_u));
}

TEST(Rule_SnippetRule, SingleDomain_Negative)
{
    auto const line = "~example1.com#$#log 'hello'";

    auto const rule = parse_rule<SnippetRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_FALSE(rule->match("http://example1.com"_u));
    EXPECT_TRUE(rule->match("http://example2.com"_u));
}

TEST(Rule_SnippetRule, MultipleDomain_PositiveOnly)
{
    auto const line = "example1.com,example2.com#$#log 'hello'";

    auto const rule = parse_rule<SnippetRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_TRUE(rule->match("http://example1.com"_u));
    EXPECT_TRUE(rule->match("http://example2.com"_u));
    EXPECT_FALSE(rule->match("http://example3.com"_u));
}

TEST(Rule_SnippetRule, MultipleDomain_Mix)
{
    auto const line = "example.com,~sub.example.com#$#log 'hello'";

    auto const rule = parse_rule<SnippetRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_TRUE(rule->match("http://example.com"_u));
    EXPECT_TRUE(rule->match("http://foo.example.com"_u));
    EXPECT_FALSE(rule->match("http://sub.example.com"_u));
}

TEST(Rule_SnippetRule, MultipleDomain_NegativeOnly)
{
    auto const line = "~example1.com,~example2.com#$#log 'hello'";

    auto const rule = parse_rule<SnippetRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_FALSE(rule->match("http://example1.com"_u));
    EXPECT_FALSE(rule->match("http://example2.com"_u));
    EXPECT_TRUE(rule->match("http://example3.com"_u));
}
