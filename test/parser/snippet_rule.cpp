#include "parser/parser.hpp"

#include "core/string_range.hpp"
#include "rule/snippet_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_SnippetRule, NoDomain)
{
    auto const line = "#$#log 'hello'";

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<SnippetRule*>(rule.get());
    ASSERT_TRUE(filter);

    EXPECT_EQ("log 'hello'", filter->snippet());
    EXPECT_FALSE(filter->domains());
}

TEST(Parser_SnippetRule, SinglePositiveDomain)
{
    auto const line = "example.com#$#log 'hello'";

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<SnippetRule*>(rule.get());
    ASSERT_TRUE(filter);

    EXPECT_EQ("log 'hello'", filter->snippet());

    auto* const domains = filter->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_EQ("example.com", (*domains)[0]);
}

TEST(Parser_SnippetRule, SingleNegativeDomain)
{
    auto const line = "~example.com#$#log 'hello'";

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<SnippetRule*>(rule.get());
    ASSERT_TRUE(filter);

    EXPECT_EQ("log 'hello'", filter->snippet());

    auto* const domains = filter->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_EQ("~example.com", (*domains)[0]);
}

TEST(Parser_SnippetRule, MultipleDomain)
{
    auto const line = "example.com,~sub.example.com#$#log 'hello'";

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<SnippetRule*>(rule.get());
    ASSERT_TRUE(filter);

    EXPECT_EQ("log 'hello'", filter->snippet());

    auto* const domains = filter->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(2, domains->size());
    EXPECT_EQ("example.com", (*domains)[0]);
    EXPECT_EQ("~sub.example.com", (*domains)[1]);
}
