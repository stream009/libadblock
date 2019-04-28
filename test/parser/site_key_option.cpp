#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "rule/filter_option.hpp"
#include "rule/filter_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_FilterOption, SiteKey_Basic)
{
    auto const line = "adblock$sitekey=abcdefg"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::SiteKey));

    auto* const siteKeys = rule->siteKeys();
    ASSERT_TRUE(siteKeys);
    ASSERT_EQ(1, siteKeys->size());
    EXPECT_EQ("abcdefg"_r, (*siteKeys)[0]);
}

TEST(Parser_FilterOption, SiteKey_MultipleSiteKey)
{
    auto const line = "adblock$sitekey=abcdefg|hijklmn"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::SiteKey));

    auto* const siteKeys = rule->siteKeys();
    ASSERT_TRUE(siteKeys);
    ASSERT_EQ(2, siteKeys->size());
    EXPECT_EQ("abcdefg"_r, (*siteKeys)[0]);
    EXPECT_EQ("hijklmn"_r, (*siteKeys)[1]);
}
