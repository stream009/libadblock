#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "rule/snippet_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_SnippetRule, NoDomain)
{
    auto const line = "#$#log 'hello'";

    auto const rule = parse_rule<SnippetRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_EQ("log 'hello'", rule->snippet());
    EXPECT_FALSE(rule->domains());
}

TEST(Parser_SnippetRule, SinglePositiveDomain)
{
    auto const line = "example.com#$#log 'hello'";

    auto const rule = parse_rule<SnippetRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_EQ("log 'hello'", rule->snippet());

    auto* const domains = rule->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_EQ("example.com", (*domains)[0]);
}

TEST(Parser_SnippetRule, SingleNegativeDomain)
{
    auto const line = "~example.com#$#log 'hello'";

    auto const rule = parse_rule<SnippetRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_EQ("log 'hello'", rule->snippet());

    auto* const domains = rule->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_EQ("~example.com", (*domains)[0]);
}

TEST(Parser_SnippetRule, MultipleDomain)
{
    auto const line = "example.com,~sub.example.com#$#log 'hello'";

    auto const rule = parse_rule<SnippetRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_EQ("log 'hello'", rule->snippet());

    auto* const domains = rule->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(2, domains->size());
    EXPECT_EQ("example.com", (*domains)[0]);
    EXPECT_EQ("~sub.example.com", (*domains)[1]);
}
