#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "rule/filter_option.hpp"
#include "rule/filter_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_FilterOption, Script_Normal)
{
    auto const line = "adblock$script"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::Script));
}

TEST(Parser_FilterOption, Script_Inverse)
{
    auto const line = "adblock$~script"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::ScriptInv));
}

TEST(Parser_FilterOption, Image_Normal)
{
    auto const line = "adblock$image"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::Image));
}

TEST(Parser_FilterOption, Image_Inverse)
{
    auto const line = "adblock$~image"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::ImageInv));
}

TEST(Parser_FilterOption, StyleSheet_Normal)
{
    auto const line = "adblock$stylesheet"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::StyleSheet));
}

TEST(Parser_FilterOption, StyleSheet_Inverse)
{
    auto const line = "adblock$~stylesheet"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::StyleSheetInv));
}

TEST(Parser_FilterOption, Object_Normal)
{
    auto const line = "adblock$object"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::Object));
}

TEST(Parser_FilterOption, Object_Inverse)
{
    auto const line = "adblock$~object"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::ObjectInv));
}

TEST(Parser_FilterOption, XmlHttpRequest_Normal)
{
    auto const line = "adblock$xmlhttprequest"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::XmlHttpRequest));
}

TEST(Parser_FilterOption, XmlHttpRequest_Inverse)
{
    auto const line = "adblock$~xmlhttprequest"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::XmlHttpRequestInv));
}

TEST(Parser_FilterOption, ObjectSubrequest_Normal)
{
    auto const line = "adblock$object-subrequest"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::ObjectSubrequest));
}

TEST(Parser_FilterOption, ObjectSubrequest_Inverse)
{
    auto const line = "adblock$~object-subrequest"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::ObjectSubrequestInv));
}

TEST(Parser_FilterOption, Subdocument_Normal)
{
    auto const line = "adblock$subdocument"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::Subdocument));
}

TEST(Parser_FilterOption, Subdocument_Inverse)
{
    auto const line = "adblock$~subdocument"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::SubdocumentInv));
}

TEST(Parser_FilterOption, Other_Normal)
{
    auto const line = "adblock$other"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::Other));
}

TEST(Parser_FilterOption, Other_Inverse)
{
    auto const line = "adblock$~other"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::OtherInv));
}

TEST(Parser_FilterOption, ThirdParty_Normal)
{
    auto const line = "adblock$third-party"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::ThirdParty));
}

TEST(Parser_FilterOption, ThirdParty_Inverse)
{
    auto const line = "adblock$~third-party"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::SameOrigin));
}

TEST(Parser_FilterOption, MatchCase)
{
    auto const line = "adblock$match-case"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::MatchCase));
}

TEST(Parser_FilterOption, Collapse_Normal)
{
    auto const line = "adblock$collapse"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::AlwaysCollapse));
}

TEST(Parser_FilterOption, Collapse_Inverse)
{
    auto const line = "adblock$~collapse"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::NeverCollapse));
}

TEST(Parser_FilterOption, Popup)
{
    auto const line = "adblock$popup"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::Popup));
}

TEST(Parser_FilterOption, Media_Normal)
{
    auto const line = "adblock$media"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::Media));
}

TEST(Parser_FilterOption, Media_Inverse)
{
    auto const line = "adblock$~media"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::MediaInv));
}

TEST(Parser_FilterOption, Font_Normal)
{
    auto const line = "adblock$font"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::Font));
}

TEST(Parser_FilterOption, Font_Inverse)
{
    auto const line = "adblock$~font"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(1, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::FontInv));
}

TEST(Parser_FilterOption, Multiple)
{
    auto const line = "adblock$script,image"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(2, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::Script));
    EXPECT_TRUE(rule->hasOption(FilterOption::Image));
}

TEST(Parser_FilterOption, Multiple_Inverse)
{
    auto const line = "adblock$~script,~image"_r;

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(2, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::ScriptInv));
    EXPECT_TRUE(rule->hasOption(FilterOption::ImageInv));
}

TEST(Parser_FilterOption, Rewrite)
{
    auto const line = "/https:\\/\\/foo.com\\/file1.txt/$rewrite=/file2.txt"_r;

    EXPECT_THROW(parse_rule<FilterRule>(line), parse_error);
}

TEST(Parser_FilterOption, StrangeCombination1)
{
    auto const line = "adblock$script,~image";

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(2, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::Script));
    EXPECT_TRUE(rule->hasOption(FilterOption::ImageInv));
}

TEST(Parser_FilterOption, StrangeCombination2)
{
    auto const line = "adblock$~script,image";

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(2, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::ScriptInv));
    EXPECT_TRUE(rule->hasOption(FilterOption::Image));
}

TEST(Parser_FilterOption, StrangeCombination3)
{
    auto const line = "adblock$script,~script";

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(2, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::Script));
    EXPECT_TRUE(rule->hasOption(FilterOption::ScriptInv));
}

TEST(Parser_FilterOption, StrangeCombination4)
{
    auto const line = "adblock$~script,script";

    auto const rule = parse_rule<FilterRule>(line);
    ASSERT_TRUE(rule);

    ASSERT_EQ(2, rule->numOptions());
    EXPECT_TRUE(rule->hasOption(FilterOption::ScriptInv));
    EXPECT_TRUE(rule->hasOption(FilterOption::Script));
}
