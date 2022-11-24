#include "../parse_rule.hpp"

#include "core/filter_rule_base.hpp"
#include "core/json.hpp"
#include "core/snippet_rule_base.hpp"
#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/snippet_rule.hpp"

#include <gtest/gtest.h>

using namespace adblock;

template<typename T, typename U>
static bool
contains(std::vector<T> const& vec, U const& value)
{
    return std::find_if(
        vec.begin(), vec.end(),
        [&](auto& v) { return v == value; }
    ) != vec.end();
}

TEST(Core_SnippetRuleBase, NoDomain)
{
    auto const line = "#$#log 'hello'";

    auto const rule = parse_rule<SnippetRule>(line);
    ASSERT_TRUE(rule);

    FilterRuleBase fb;
    SnippetRuleBase rb { fb };
    rb.put(*rule);

    auto const& rules = rb.lookup("http://example.com"_u);
    ASSERT_EQ(1, rules.size());
    EXPECT_EQ(rule.get(), rules[0]);
}

TEST(Core_SnippetRuleBase, SingleDomain_Positive)
{
    auto const line1 = "#$#log 'hello'";
    auto const rule1 = parse_rule<SnippetRule>(line1);
    ASSERT_TRUE(rule1);

    auto const line2 = "example1.com#$#log 'hello'";
    auto const rule2 = parse_rule<SnippetRule>(line2);
    ASSERT_TRUE(rule2);

    FilterRuleBase fb;
    SnippetRuleBase rb { fb };
    rb.put(*rule1);
    rb.put(*rule2);

    {
        auto const& rules = rb.lookup("http://example1.com"_u);
        ASSERT_EQ(2, rules.size());
        EXPECT_TRUE(contains(rules, rule1.get()));
        EXPECT_TRUE(contains(rules, rule2.get()));
    }
    {
        auto const& rules = rb.lookup("http://example2.com"_u);
        ASSERT_EQ(1, rules.size());
        EXPECT_TRUE(contains(rules, rule1.get()));
    }
}

TEST(Core_SnippetRuleBase, SingleDomain_Negative)
{
    auto const line1 = "#$#log 'hello'";
    auto const rule1 = parse_rule<SnippetRule>(line1);
    ASSERT_TRUE(rule1);

    auto const line2 = "~example1.com#$#log 'hello'";
    auto const rule2 = parse_rule<SnippetRule>(line2);
    ASSERT_TRUE(rule2);

    FilterRuleBase fb;
    SnippetRuleBase rb { fb };
    rb.put(*rule1);
    rb.put(*rule2);

    {
        auto const& rules = rb.lookup("http://example1.com"_u);
        ASSERT_EQ(1, rules.size());
        EXPECT_TRUE(contains(rules, rule1.get()));
    }
    {
        auto const& rules = rb.lookup("http://example2.com"_u);
        ASSERT_EQ(2, rules.size());
        EXPECT_TRUE(contains(rules, rule1.get()));
        EXPECT_TRUE(contains(rules, rule2.get()));
    }
}

TEST(Core_SnippetRuleBase, WhiteList)
{
    FilterRuleBase fb;
    SnippetRuleBase rb { fb };

    auto const line1 = "#$#log 'hello'";
    auto const rule1 = parse_rule<SnippetRule>(line1);
    ASSERT_TRUE(rule1);
    rb.put(*rule1);

    auto const line2 = "example1.com#$#log 'hello'";
    auto const rule2 = parse_rule<SnippetRule>(line2);
    ASSERT_TRUE(rule2);
    rb.put(*rule2);

    auto const line3 = "@@||example1.com$document";
    auto const rule3 = parse_rule<FilterRule>(line3);
    ASSERT_TRUE(rule3);
    fb.put(*rule3);

    {
        auto const& rules = rb.lookup("http://example1.com"_u);
        ASSERT_EQ(0, rules.size());
    }
    {
        auto const& rules = rb.lookup("http://example2.com"_u);
        ASSERT_EQ(1, rules.size());
        EXPECT_TRUE(contains(rules, rule1.get()));
    }
}

TEST(Core_SnippetRuleBase, SpecificOnly)
{
    FilterRuleBase fb;
    SnippetRuleBase rb { fb };

    auto const line1 = "#$#log 'hello'";
    auto const rule1 = parse_rule<SnippetRule>(line1);
    ASSERT_TRUE(rule1);
    rb.put(*rule1);

    auto const line2 = "example1.com#$#log 'hello'";
    auto const rule2 = parse_rule<SnippetRule>(line2);
    ASSERT_TRUE(rule2);
    rb.put(*rule2);

    auto const line3 = "@@||example1.com$genericblock";
    auto const rule3 = parse_rule<FilterRule>(line3);
    ASSERT_TRUE(rule3);
    fb.put(*rule3);

    {
        auto const& rules = rb.lookup("http://example1.com"_u);
        ASSERT_EQ(1, rules.size());
        EXPECT_TRUE(contains(rules, rule2.get()));
    }
    {
        auto const& rules = rb.lookup("http://example2.com"_u);
        ASSERT_EQ(1, rules.size());
        EXPECT_TRUE(contains(rules, rule1.get()));
    }
}

TEST(Core_SnippetRuleBase, Statistics)
{
    FilterRuleBase fb;
    SnippetRuleBase rb { fb };

    auto const line1 = "#$#log 'hello'";
    auto const rule1 = parse_rule<SnippetRule>(line1);
    ASSERT_TRUE(rule1);
    rb.put(*rule1);

    auto const line2 = "example1.com#$#log 'hello'";
    auto const rule2 = parse_rule<SnippetRule>(line2);
    ASSERT_TRUE(rule2);
    rb.put(*rule2);

    auto const stats = rb.statistics();
    EXPECT_EQ(1, to_number(stats.at("Generic snippet rule")));
    EXPECT_EQ(1, to_number(stats.at("Specific snippet rule")));
    EXPECT_EQ(2, to_number(stats.at("Total")));
}

TEST(Core_SnippetRuleBase, Clear)
{
    FilterRuleBase fb;
    SnippetRuleBase rb { fb };

    auto const line1 = "#$#log 'hello'";
    auto const rule1 = parse_rule<SnippetRule>(line1);
    ASSERT_TRUE(rule1);
    rb.put(*rule1);

    auto const line2 = "example1.com#$#log 'hello'";
    auto const rule2 = parse_rule<SnippetRule>(line2);
    ASSERT_TRUE(rule2);
    rb.put(*rule2);

    auto const before = rb.statistics();
    EXPECT_EQ(1, to_number(before.at("Generic snippet rule")));
    EXPECT_EQ(1, to_number(before.at("Specific snippet rule")));
    EXPECT_EQ(2, to_number(before.at("Total")));

    rb.clear();

    auto const after = rb.statistics();
    EXPECT_EQ(0, to_number(after.at("Total")));
}
