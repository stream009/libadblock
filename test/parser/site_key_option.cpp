#include "parser/parser.hpp"

#include "core/string_range.hpp"
#include "rule/filter_option.hpp"
#include "rule/filter_rule.hpp"

#include <memory>

#include <boost/algorithm/string.hpp>

#include <gtest/gtest.h>

using namespace adblock;

namespace ba = boost::algorithm;

TEST(Parser_FilterOption, SiteKey_Basic)
{
    auto const& line = "adblock$sitekey=abcdefg"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::SiteKey));

    auto* const siteKeys = filter->siteKeys();
    ASSERT_TRUE(siteKeys);
    ASSERT_EQ(1, siteKeys->size());
    EXPECT_TRUE(ba::equals("abcdefg"_r, (*siteKeys)[0]));
}

TEST(Parser_FilterOption, SiteKey_MultipleSiteKey)
{
    auto const& line = "adblock$sitekey=abcdefg|hijklmn"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::SiteKey));

    auto* const siteKeys = filter->siteKeys();
    ASSERT_TRUE(siteKeys);
    ASSERT_EQ(2, siteKeys->size());
    EXPECT_TRUE(ba::equals("abcdefg"_r, (*siteKeys)[0]));
    EXPECT_TRUE(ba::equals("hijklmn"_r, (*siteKeys)[1]));
}
