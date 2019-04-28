#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "rule/filter_option.hpp"
#include "rule/filter_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_FilterOption, Domain_Basic)
{
    auto const line = "adblock$domain=adblock.org"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    ASSERT_TRUE(rule->hasOption(FilterOption::Domain));

    auto* const domains = rule->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_EQ("adblock.org", (*domains)[0]);
}

TEST(Parser_FilterOption, Domain_ExcludeDomain)
{
    auto const line = "adblock$domain=~adblock.org"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    ASSERT_TRUE(rule->hasOption(FilterOption::Domain));

    auto* const domains = rule->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_EQ("~adblock.org", (*domains)[0]);
}

TEST(Parser_FilterOption, Domain_MultipleDomain1)
{
    auto const line = "adblock$domain=adblock.org|google.com"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    ASSERT_TRUE(rule->hasOption(FilterOption::Domain));

    auto* const domains = rule->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(2, domains->size());
    EXPECT_EQ("adblock.org", (*domains)[0]);
    EXPECT_EQ("google.com", (*domains)[1]);
}

TEST(Parser_FilterOption, Domain_MultipleDomain2)
{
    auto const line = "adblock$domain=adblock.org|~google.com"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    ASSERT_TRUE(rule->hasOption(FilterOption::Domain));

    auto* const domains = rule->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(2, domains->size());
    EXPECT_EQ("adblock.org", (*domains)[0]);
    EXPECT_EQ("~google.com", (*domains)[1]);
}
