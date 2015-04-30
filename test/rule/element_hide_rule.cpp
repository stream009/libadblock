#include "type.hpp"
#include "option.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "pattern/basic_match_pattern.hpp"

#include "../mock_context.hpp"

#include <memory>

#include <boost/range/iterator_range.hpp>

#include <gtest/gtest.h>

using namespace adblock;

TEST(BasicElementHideRule, Basic)
{
    const auto &cssSelector = "div"_r;
    const BasicElementHideRule rule { cssSelector, boost::none };

    EXPECT_EQ(cssSelector, rule.cssSelector());
    EXPECT_TRUE(rule.includeDomains().empty());
    EXPECT_TRUE(rule.excludeDomains().empty());

    const auto &uri = "http://adblock.org"_u;
    EXPECT_TRUE(rule.match(uri));
}

TEST(BasicElementHideRule, Domain)
{
    const auto &cssSelector = "div"_r;
    const std::vector<StringRange> domains {
        "adblock.org"_r,
        "~special.adblock.org"_r,
    };
    const BasicElementHideRule rule { cssSelector, domains };

    EXPECT_EQ(1, rule.includeDomains().size());
    EXPECT_EQ(1, rule.excludeDomains().size());

    EXPECT_TRUE(rule.match("http://www.adblock.org/something.html"_u));
    EXPECT_FALSE(
        rule.match("http://something.special.adblock.org/index.html"_u));
    EXPECT_FALSE(rule.match("http://www.google.com"_u));
}

TEST(BasicElementHideRule, MultipleDomains)
{
    const auto &cssSelector = "div"_r;
    const std::vector<StringRange> domains {
        "adblock.org"_r,
        "~special.adblock.org"_r,
        "google.com"_r,
        "~mail.google.com"_r,
    };
    const BasicElementHideRule rule { cssSelector, domains };

    EXPECT_EQ(2, rule.includeDomains().size());
    EXPECT_EQ(2, rule.excludeDomains().size());

    EXPECT_TRUE(rule.match("http://www.adblock.org/something.html"_u));
    EXPECT_FALSE(
        rule.match("http://something.special.adblock.org/index.html"_u));

    EXPECT_TRUE(rule.match("http://www.google.com"_u));
    EXPECT_FALSE(rule.match("http://mail.google.com/"_u));

    EXPECT_FALSE(rule.match("http://www.facebook.com"_u));
}
