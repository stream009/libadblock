#include "parser/parser.hpp"

#include "option.hpp"
#include "option/site_key_option.hpp"
#include "rule/filter_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_FilterOption, SiteKey_Basic)
{
    auto const& line = "adblock$sitekey=abcdefg"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(1, options.size());

    auto* const option = dynamic_cast<SiteKeyOption*>(options.front().get());
    ASSERT_TRUE(option);
    ASSERT_EQ(1, option->siteKeys().size());
    EXPECT_EQ("abcdefg"_r, option->siteKeys().front());
}

TEST(Parser_FilterOption, SiteKey_MultipleSiteKey)
{
    auto const& line = "adblock$sitekey=abcdefg|hijklmn"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(1, options.size());

    auto* const option = dynamic_cast<SiteKeyOption*>(options.front().get());
    ASSERT_TRUE(option);
    ASSERT_EQ(2, option->siteKeys().size());
    EXPECT_EQ("abcdefg"_r, option->siteKeys()[0]);
    EXPECT_EQ("hijklmn"_r, option->siteKeys()[1]);
}
