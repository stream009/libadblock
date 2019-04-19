#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "core/type.hpp"

#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Rule_BasicElementHideRule, Basic)
{
    auto const& line = "##div"_r;

    auto const rule = parse_rule<BasicElementHideRule>(line);
    ASSERT_TRUE(rule);

    auto const& uri = "http://adblock.org"_u;
    EXPECT_TRUE(rule->match(uri));
}

TEST(Rule_BasicElementHideRule, Domain)
{
    auto const& line = "adblock.org##div"_r;

    auto const rule = parse_rule<BasicElementHideRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_TRUE(rule->match("http://www.adblock.org/something.html"_u));
    EXPECT_FALSE(rule->match("http://www.google.com"_u));
}

TEST(Rule_BasicElementHideRule, MultipleDomains)
{
    auto const& line = "adblock.org,~special.adblock.org,google.com,~mail.google.com##div"_r;

    auto const rule = parse_rule<BasicElementHideRule>(line);
    ASSERT_TRUE(rule);

    EXPECT_TRUE(rule->match("http://www.adblock.org/something.html"_u));
    EXPECT_FALSE(
        rule->match("http://something.special.adblock.org/index.html"_u));

    EXPECT_TRUE(rule->match("http://www.google.com"_u));
    EXPECT_FALSE(rule->match("http://mail.google.com/"_u));

    EXPECT_FALSE(rule->match("http://www.facebook.com"_u));
}

//TODO test exception rule
