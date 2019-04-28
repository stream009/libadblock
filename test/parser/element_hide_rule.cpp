#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "rule/extended_element_hide_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_BasicElementHideRule, Basic)
{
    auto const line = "###ad"_r;

    auto const rule = parse_rule<BasicElementHideRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_EQ("#ad"_r, rule->cssSelector());
    EXPECT_FALSE(rule->domains());
}

TEST(Parser_BasicElementHideRule, SingleDomain1)
{
    auto const line = "adblock.org###ad"_r;

    auto const rule = parse_rule<BasicElementHideRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_EQ("#ad"_r, rule->cssSelector());

    auto* const domains = rule->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_EQ("adblock.org"_r, (*domains)[0]);
}

TEST(Parser_BasicElementHideRule, SingleDomain2)
{
    auto const line = "~adblock.org###ad"_r;

    auto const rule = parse_rule<BasicElementHideRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_EQ("#ad"_r, rule->cssSelector());

    auto* const domains = rule->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_EQ("~adblock.org"_r, (*domains)[0]);
}

TEST(Parser_BasicElementHideRule, MultipleDomain)
{
    auto const line = "adblock.org,~google.com,facebook.com###ad"_r;

    auto const rule = parse_rule<BasicElementHideRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_EQ("#ad"_r, rule->cssSelector());

    auto* const domains = rule->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(3, domains->size());
    EXPECT_EQ("adblock.org"_r, (*domains)[0]);
    EXPECT_EQ("~google.com"_r, (*domains)[1]);
    EXPECT_EQ("facebook.com"_r, (*domains)[2]);
}

TEST(Parser_ExceptionElementHideRule, Basic)
{
    auto const line = "#@##ad"_r;

    auto const rule = parse_rule<ExceptionElementHideRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_EQ("#ad"_r, rule->cssSelector());

    EXPECT_FALSE(rule->domains());
}

TEST(Parser_ExtendedElementHideRule, Basic)
{
    auto const line = "#?##ad"_r;

    auto const rule = parse_rule<ExtendedElementHideRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_EQ("#ad"_r, rule->cssSelector());

    EXPECT_FALSE(rule->domains());
}
