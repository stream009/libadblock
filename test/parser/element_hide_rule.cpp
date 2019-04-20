#include "parser/parser.hpp"

#include "core/string_range.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "rule/extended_element_hide_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_BasicElementHideRule, Basic)
{
    auto const& line = "###ad"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<BasicElementHideRule*>(rule.get());
    ASSERT_TRUE(filter);

    EXPECT_EQ("#ad"_r, filter->cssSelector());
    EXPECT_FALSE(filter->domains());
}

TEST(Parser_BasicElementHideRule, SingleDomain1)
{
    auto const& line = "adblock.org###ad"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<BasicElementHideRule*>(rule.get());
    ASSERT_TRUE(filter);

    EXPECT_EQ("#ad"_r, filter->cssSelector());

    auto* const domains = filter->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_EQ("adblock.org"_r, (*domains)[0]);
}

TEST(Parser_BasicElementHideRule, SingleDomain2)
{
    auto const& line = "~adblock.org###ad"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<BasicElementHideRule*>(rule.get());
    ASSERT_TRUE(filter);

    EXPECT_EQ("#ad"_r, filter->cssSelector());

    auto* const domains = filter->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(1, domains->size());
    EXPECT_EQ("~adblock.org"_r, (*domains)[0]);
}

TEST(Parser_BasicElementHideRule, MultipleDomain)
{
    auto const& line = "adblock.org,~google.com,facebook.com###ad"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<BasicElementHideRule*>(rule.get());
    ASSERT_TRUE(filter);

    EXPECT_EQ("#ad"_r, filter->cssSelector());

    auto* const domains = filter->domains();
    ASSERT_TRUE(domains);
    ASSERT_EQ(3, domains->size());
    EXPECT_EQ("adblock.org"_r, (*domains)[0]);
    EXPECT_EQ("~google.com"_r, (*domains)[1]);
    EXPECT_EQ("facebook.com"_r, (*domains)[2]);
}

TEST(Parser_ExceptionElementHideRule, Basic)
{
    auto const& line = "#@##ad"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<ExceptionElementHideRule*>(rule.get());
    ASSERT_TRUE(filter);

    EXPECT_EQ("#ad"_r, filter->cssSelector());

    EXPECT_FALSE(filter->domains());
}

TEST(Parser_ExtendedElementHideRule, Basic)
{
    auto const& line = "#?##ad"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<ExtendedElementHideRule*>(rule.get());
    ASSERT_TRUE(filter);

    EXPECT_EQ("#ad"_r, filter->cssSelector());

    EXPECT_FALSE(filter->domains());
}
